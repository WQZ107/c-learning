# -*- coding: utf-8 -*-
# m1_check.py — 🏁M1 接收端:解析 PCAN-View 导出的 .trc,还原协议数据 + 识别急刹
# 用法:python m1_check.py 你的trace文件.trc
# 兼容 PCAN-View 两种常见文本格式(带[长度]的复制文本 / trc v1.1 / trc v2.x)
import struct, re, sys

def f32(b):                                    # 0x100/101:float 小端
    return struct.unpack('<f', bytes(b))[0]

def s16(b):                                    # 0x102/104~106/300:int16 大端
    v = (b[0] << 8) | b[1]
    return v - 0x10000 if v >= 0x8000 else v

PATTERNS = [
    # PCAN-View trc v1.1:  "    1)   12.0  Rx   0104  2  FF 9F"  (ID 无 h 后缀)
    re.compile(r'(?:\d+\)\s+[\d.]+\s+)?Rx\s+([0-9A-Fa-f]{2,8})h?\s+(\d)\s+((?:[0-9A-Fa-f]{2}\s+)+)'),
    # 复制文本格式: "100h  [4]  00 00 00 00"
    re.compile(r'([0-9A-Fa-f]{2,8})h\s+\[\s*(\d+)\s*\]\s*((?:[0-9A-Fa-f]{2}[\s:]+)+)'),
    # trc v2.x: "100h Rx 4 00 00 00 00"
    re.compile(r'([0-9A-Fa-f]{2,8})h\s+(?:Rx|Tx)\s+(\d)\s+((?:[0-9A-Fa-f]{2}\s+)+)'),
]

frames, alerts, unknown, total = {}, [], 0, 0
for line in open(sys.argv[1], errors='replace'):
    if not re.search(r'Rx\s+[0-9A-Fa-f]{2,8}', line):
        continue
    total += 1
    m = None
    for p in PATTERNS:
        m = p.search(line)
        if m: break
    if not m:
        unknown += 1
        continue
    cid = int(m.group(1), 16)
    n = int(m.group(2))
    data = [int(x, 16) for x in re.findall(r'[0-9A-Fa-f]{2}', m.group(3))][:n]
    if len(data) < n: continue
    if   cid == 0x100 and n == 4: frames.setdefault('0x100 lat', []).append(f32(data))
    elif cid == 0x101 and n == 4: frames.setdefault('0x101 lon', []).append(f32(data))
    elif cid == 0x102 and n == 2: frames.setdefault('0x102 spd(km/h)', []).append(s16(data))
    elif cid == 0x103 and n == 2: frames.setdefault('0x103 temp(C)', []).append(s16(data)/10)
    elif cid == 0x108 and n == 2: frames.setdefault('0x108 humi(%)', []).append(s16(data)/10)   # 块K 2026-10-02 新增(DHT11)
    elif cid in (0x104, 0x105, 0x106) and n == 2:
        frames.setdefault('0x10%d acc(g)' % (cid - 0x104 + 4), []).append(s16(data)/100)
    elif cid == 0x107 and n == 2: frames.setdefault('0x107 sat/flag', []).append('%d/%d' % (data[0], data[1]))
    elif cid == 0x300 and n == 2: alerts.append(s16(data)/100)

print('=== 🏁M1 接收端还原(来自 %s) ===' % sys.argv[1])
for k in sorted(frames):
    v = frames[k]
    if isinstance(v[0], str):
        print('%-18s n=%-5d 最新=%s' % (k, len(v), v[-1]))
    else:
        print('%-18s n=%-5d 最新=%-14.6g min=%-14.6g max=%g' % (k, len(v), v[-1], min(v), max(v)))
print('=== 急刹事件(0x300) ===')
print('%d 次:' % len(alerts), ['%.2fg' % a for a in alerts] or '无')
if unknown:
    print('⚠️ 有 %d 行含 CAN-ID 但没解析成功(格式可能不同,把文件前 10 行发我)' % unknown)
if total == 0:
    print('⚠️ 文件里没发现任何带 ID 的行——确认导出的是 trace 文件,且内容非空')

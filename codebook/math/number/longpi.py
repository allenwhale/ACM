class LongPI:
    def __init__(self, L):
        n = int(L / 1.39793 + 1)
        scale = 10 ** L
        self.PI = 0
        w = 16 * 5 * scale
        v = 4 * 239 * scale
        for i in range(1, n + 1):
            w //= 25
            v //= 57121
            q = (w - v) // (2 * i - 1)
            self.PI += q if i % 2 == 1 else -q

    def __str__(self):
        s = str(self.PI)
        return "%c.%s" % (s[0], s[1:])            
print(LongPI(100), len(str(LongPI(100))))

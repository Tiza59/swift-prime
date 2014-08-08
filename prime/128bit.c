//
//  128bit.c
//  prime
//
//  Created by Dan Kogai on 8/6/14.
//  Copyright (c) 2014 Dan Kogai. All rights reserved.
//
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
uint64_t c_powmod(uint64_t b64, uint64_t p64, uint64_t m64) {
    __uint128_t b128, r128;
    for (r128 = 1, b128 = b64; p64 > 0; p64 >>= 1) {
        if ((p64 & 1) == 1) { r128 = (r128 * b128) % m64; }
        b128 = (b128 * b128) % m64;
    }
    return (uint64_t)r128;
}
int c_mrtest(uint64_t n64, uint64_t b64) {
    if (n64 < 2)  { return 0; }
    if ((n64 & 1) == 0) { return n64 == 2; }
    uint64_t d64 = n64-1;
    while((d64 & 1) == 0) { d64 >>= 1; }
    uint64_t    t64 = d64;
    __uint128_t y128 = c_powmod(b64, t64, n64);
    while (t64 != n64-1 && y128 != 1 && y128 != n64-1) {
        y128 = (y128 * y128) % n64;
        t64 <<= 1;
    }
    return y128 == n64-1 || (t64 & 1) == 1;
}
uint64_t c_sqaddmod(uint64_t n64, uint64_t p64, uint64_t m64) {
    __uint128_t r128 = n64 * n64 + p64;
    return (uint64_t)(r128 % m64);
}
uint64_t gcd(uint64_t m, uint64_t n){
    if (n == 0) { return m; }
    if (m < n) { return gcd(n, m); }
    uint64_t r = m < (n << 1) ? m - n : m % n;
    return r == 0 ? n : gcd(n, r);
}
uint64_t c_ipow(uint64_t b, uint64_t n) {
    uint64_t result = 1;
    for (; n > 0; n >>= 1, b *= b) {
        if ((n & 1) == 1) { result *= b; }
    }
    return result;
}
uint64_t isqrt(uint64_t n) {
    if (n == 0) { return 0; }
    if (n == 1) { return 1; }
    if (n == 18446744073709551615ULL) { return 4294967295; }
    uint64_t xk = n;
    do {
        uint64_t xk1 = (xk + n / xk) / 2;
        if (xk1 >= xk) { return xk; }
        xk = xk1;
    } while(1);
}
uint64_t c_pbrho(uint64_t n, uint64_t l, int c) {
    uint64_t x = 2, y = 2, q = 1, d = 1;
    if (c < 0) {
        c = -c;
        x = y = arc4random();
    }
    int i, j;
    for (i = 1, j = 2; i < l; i++) {
        x  = c_sqaddmod(x, c, n);
        q *= x < y ? y - x : x - y; q %= n;
        d = gcd(n, q);
        if (d != 1) {
            return d == n ? 1 : d;
        }
        if (i % j == 0) {
            y = x;
            j += j;
        }
    }
    return 1;
}
uint64_t isqrt2(uint64_t n, uint64_t w) {
    if (n == 0) { return 0; }
    if (n == 1) { return 1; }
    if (n == 18446744073709551615ULL) { return 4294967295; }
    __uint128_t xk = n * w;
    do {
        __uint128_t xk1 = (xk + n / xk) / 2;
        if (xk1 >= xk) { return (uint64_t)xk; }
        xk = xk1;
    } while(1);
}
#define knp2(k,n,p0) \
  (uint64_t)((__uint128_t)((k)*(n))-(__uint128_t)((p0)*(p0)))
#define qh(q1) ((q1) & 1) == 0 ? (q1) >> 1 : (q1)
uint64_t c_squfof(uint64_t n, uint64_t k) {
    if (n < 2)        { return 1; }
    if ((n & 1) == 0) { return 2; }
    uint64_t rnk = isqrt(n);
    if (rnk * rnk == n) return rnk;
    rnk = isqrt2(n, k);
    int l = (int)isqrt(2*isqrt(n)), imax = 4 * l;
    uint64_t p0, p1 = 0, q0, q1, q2, b, rq = 1;
    p0 = rnk, q0 = 1; q1 = knp2(k, n, p0);
    int i;
    for (i = 1; i < imax; i++, p0 = p1, q0 = q1, q1 = q2) {
        b = (rnk + p0)/q1;
        p1 = b*q1 - p0;
        q2 = q0 + b*(p0 - p1);
        rq = isqrt(q2);
        if (rq * rq == q2) break;
    }
    if (i == imax) return 1;
    b = (rnk - p1)/rq, p0 = b*rq + p1,
    q0 = rq, q1 = knp2(k, n, p0) / q0;
    for (i = 1; p0 != p1; i++, p0 = p1, q0 = q1, q1 = q2) {
        b = (rnk + p0)/q1;
        p1 = b*q1 - p0;
        q2 = q0 + b*(p0 - p1);
    }
    return (q1 & 1) == 0 ? q1 >> 1 : q1;
}

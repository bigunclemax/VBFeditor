/* LZSS encoder-decoder (Haruhiko Okumura; public domain)
 * with SmartGauges's modifications */

#ifndef LZSS_HPP_INCLUDED
#define LZSS_HPP_INCLUDED

#include <vector>
#include <cstdint>
#include <cstdio>

namespace lzss
{

constexpr int EI = 10;  /* typically 10..13 */
constexpr int EJ = 4;  /* typically 4..5 */
constexpr int P = 1;  /* If match length <= P then output one character */
constexpr int N = (1 << EI);  /* buffer size */
constexpr int F = ((1 << EJ) + P);  /* lookahead buffer size */

constexpr int ROUNDUP_DEC_RATIO = ((F * 8) + ((EI + EJ) - 1)) / (EI + EJ);
constexpr int EXPECT_DEC_RATIO = (ROUNDUP_DEC_RATIO + (2 - 1)) / 2;

using std::vector;

struct enc_ctx {
    int bit_buffer = 0;
    int bit_mask = 128;
};

struct dec_ctx {
    int idx = 0;
    int buf = 0;
    int mask = 0;
};

void putbit1(enc_ctx &ctx, vector<uint8_t> &data)
{
    ctx.bit_buffer |= ctx.bit_mask;
    if ((ctx.bit_mask >>= 1) == 0) {
        data.push_back(ctx.bit_buffer);
        ctx.bit_buffer = 0;  ctx.bit_mask = 128;
    }
}

void putbit0(enc_ctx &ctx, vector<uint8_t> &data)
{
    if ((ctx.bit_mask >>= 1) == 0) {
        data.push_back(ctx.bit_buffer);
        ctx.bit_buffer = 0;  ctx.bit_mask = 128;
    }
}

void flush_bit_buffer(enc_ctx &ctx, vector<uint8_t> &data)
{
    if (ctx.bit_mask != 128) {
        data.push_back(ctx.bit_buffer);
    }
}

void output1(enc_ctx &ctx, vector<uint8_t> &data, int c)
{
    int mask;

    putbit1(ctx, data);
    mask = 256;
    while (mask >>= 1) {
        if (c & mask) putbit1(ctx, data);
        else putbit0(ctx, data);
    }
}

void output2(enc_ctx &ctx, vector<uint8_t> &data, int x, int y)
{
    int mask;

    putbit0(ctx, data);
    mask = N;
    while (mask >>= 1) {
        if (x & mask) putbit1(ctx, data);
        else putbit0(ctx, data);
    }
    mask = (1 << EJ);
    while (mask >>= 1) {
        if (y & mask) putbit1(ctx, data);
        else putbit0(ctx, data);
    }
}

vector<uint8_t> encode(const vector<uint8_t> &data)
{
    unsigned char buffer[N * 2];
    vector<uint8_t> cdata;
    cdata.reserve(data.size());
    enc_ctx ctx{};
    int idx = 0;
    int i, j, f1, x, y, r, s, bufferend, c;

    for (i = 0; i < N - F; i++) buffer[i] = '\0';
    for (i = N - F; i < N * 2; i++) {
        if (idx >= data.size()) break;
        buffer[i] = data[idx++];
    }
    bufferend = i;  r = N - F;  s = 0;
    while (r < bufferend) {
        f1 = (F <= bufferend - r) ? F : bufferend - r;
        x = 0;  y = 1;  c = buffer[r];
        for (i = r - 1; i > s; i--)
            if ((s >= (r - i)) && buffer[i] == c) {
                for (j = 1; j < f1; j++)
                    if (buffer[i + j] != buffer[r + j]) break;
                if (j > y) {
                    x = i;  y = j;
                }
            }

        if (x >= (N - F))
            x -= (N - F);
        x++;

        if (y <= P) {  output1(ctx, cdata, c);  }
        else output2(ctx, cdata, x & (N - 1), y - 2);
        r += y;  s += y;
        if (r >= N * 2 - F) {
            for (i = 0; i < N; i++) buffer[i] = buffer[i + N];
            bufferend -= N;  r -= N;  s -= N;
            while (bufferend < N * 2) {
                if (idx >= data.size()) break;
                buffer[bufferend++] = data[idx++];
            }
        }
    }
    flush_bit_buffer(ctx, cdata);

    return cdata;
}

int getbit(dec_ctx &ctx, const vector<uint8_t> &data, int n) /* get n bits */
{
    int i, x;

    x = 0;
    for (i = 0; i < n; i++) {
        if (ctx.mask == 0) {

            if (ctx.idx >= data.size())
                return EOF;

            ctx.buf = data[ctx.idx++];
            ctx.mask = 128;
        }
        x <<= 1;
        if (ctx.buf & ctx.mask) x++;
        ctx.mask >>= 1;
    }
    return x;
}

vector<uint8_t> decode(const vector<uint8_t> &cdata)
{
    unsigned char buffer[N * 2];
    dec_ctx ctx{};
    vector<uint8_t> data;
    data.reserve(cdata.size() * EXPECT_DEC_RATIO);
    int i, j, k, r, c;

    r = 0;

    while ((c = getbit(ctx, cdata, 1)) != EOF) {
        if (c) {
            if ((c = getbit(ctx, cdata, 8)) == EOF) break;
            data.push_back(c);
            buffer[r++] = c;  r &= (N - 1);
        } else {
            if ((i = getbit(ctx, cdata, EI)) == EOF) break;
            if ((j = getbit(ctx, cdata, EJ)) == EOF) break;

            if (i == 0)
                break;

            i -= 1;

            for (k = 0; k <= j + 1; k++) {
                c = buffer[(i + k) & (N - 1)];
                data.push_back(c);
                buffer[r++] = c;  r &= (N - 1);
            }
        }
    }

    data.shrink_to_fit();

    return data;
}

}

#endif //LZSS_HPP_INCLUDED
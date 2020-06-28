/*
 *  Generic function for frame buffer with packed pixels of any depth.
 *
 *      Copyright (C)  1999-2005 James Simmons <jsimmons@www.infradead.org>
 *
 *  This file is subject to the terms and conditions of the GNU General Public
 *  License.  See the file COPYING in the main directory of this archive for
 *  more details.
 *
 * NOTES:
 *
 *  This is for cfb packed pixels. Iplan and such are incorporated in the
 *  drivers that need them.
 *
 *  FIXME
 *
 *  Also need to add code to deal with cards endians that are different than
 *  the native cpu endians. I also need to deal with MSB position in the word.
 *
 *  The two functions or copying forward and backward could be split up like
 *  the ones for filling, i.e. in aligned and unaligned versions. This would
 *  help moving some redundant computations and branches out of the loop, too.
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/fb.h>
#include <asm/types.h>
#include <asm/io.h>
#include "fb_draw.h"

#if BITS_PER_LONG == 32
#  define FB_WRITEL fb_writel
#  define FB_READL  fb_readl
#else
#  define FB_WRITEL fb_writeq
#  define FB_READL  fb_readq
#endif

    /*
     *  Generic bitwise copy algorithm
     */

static void
<<<<<<< HEAD
bitcpy(struct fb_info *p, unsigned long __iomem *dst, unsigned dst_idx,
		const unsigned long __iomem *src, unsigned src_idx, int bits,
=======
bitcpy(struct fb_info *p, unsigned long __iomem *dst, int dst_idx,
		const unsigned long __iomem *src, int src_idx, int bits,
>>>>>>> 671a46baf1b... some performance improvements
		unsigned n, u32 bswapmask)
{
	unsigned long first, last;
	int const shift = dst_idx-src_idx;
<<<<<<< HEAD

#if 0
	/*
	 * If you suspect bug in this function, compare it with this simple
	 * memmove implementation.
	 */
	fb_memmove((char *)dst + ((dst_idx & (bits - 1))) / 8,
		   (char *)src + ((src_idx & (bits - 1))) / 8, n / 8);
	return;
#endif
=======
	int left, right;
>>>>>>> 671a46baf1b... some performance improvements

	first = fb_shifted_pixels_mask_long(p, dst_idx, bswapmask);
	last = ~fb_shifted_pixels_mask_long(p, (dst_idx+n) % bits, bswapmask);

	if (!shift) {
		// Same alignment for source and dest

		if (dst_idx+n <= bits) {
			// Single word
			if (last)
				first &= last;
			FB_WRITEL( comp( FB_READL(src), FB_READL(dst), first), dst);
		} else {
			// Multiple destination words

			// Leading bits
			if (first != ~0UL) {
				FB_WRITEL( comp( FB_READL(src), FB_READL(dst), first), dst);
				dst++;
				src++;
				n -= bits - dst_idx;
			}

			// Main chunk
			n /= bits;
			while (n >= 8) {
				FB_WRITEL(FB_READL(src++), dst++);
				FB_WRITEL(FB_READL(src++), dst++);
				FB_WRITEL(FB_READL(src++), dst++);
				FB_WRITEL(FB_READL(src++), dst++);
				FB_WRITEL(FB_READL(src++), dst++);
				FB_WRITEL(FB_READL(src++), dst++);
				FB_WRITEL(FB_READL(src++), dst++);
				FB_WRITEL(FB_READL(src++), dst++);
				n -= 8;
			}
			while (n--)
				FB_WRITEL(FB_READL(src++), dst++);

			// Trailing bits
			if (last)
				FB_WRITEL( comp( FB_READL(src), FB_READL(dst), last), dst);
		}
	} else {
		/* Different alignment for source and dest */
		unsigned long d0, d1;
		int m;

<<<<<<< HEAD
		int const left = shift & (bits - 1);
		int const right = -shift & (bits - 1);
=======
		right = shift & (bits - 1);
		left = -shift & (bits - 1);
		bswapmask &= shift;
>>>>>>> 671a46baf1b... some performance improvements

		if (dst_idx+n <= bits) {
			// Single destination word
			if (last)
				first &= last;
			d0 = FB_READL(src);
			d0 = fb_rev_pixels_in_long(d0, bswapmask);
			if (shift > 0) {
				// Single source word
<<<<<<< HEAD
				d0 <<= left;
			} else if (src_idx+n <= bits) {
				// Single source word
				d0 >>= right;
=======
				d0 >>= right;
			} else if (src_idx+n <= bits) {
				// Single source word
				d0 <<= left;
>>>>>>> 671a46baf1b... some performance improvements
			} else {
				// 2 source words
				d1 = FB_READL(src + 1);
				d1 = fb_rev_pixels_in_long(d1, bswapmask);
<<<<<<< HEAD
				d0 = d0 >> right | d1 << left;
=======
				d0 = d0<<left | d1>>right;
>>>>>>> 671a46baf1b... some performance improvements
			}
			d0 = fb_rev_pixels_in_long(d0, bswapmask);
			FB_WRITEL(comp(d0, FB_READL(dst), first), dst);
		} else {
			// Multiple destination words
			/** We must always remember the last value read, because in case
			SRC and DST overlap bitwise (e.g. when moving just one pixel in
			1bpp), we always collect one full long for DST and that might
			overlap with the current long from SRC. We store this value in
			'd0'. */
			d0 = FB_READL(src++);
			d0 = fb_rev_pixels_in_long(d0, bswapmask);
			// Leading bits
			if (shift > 0) {
				// Single source word
				d1 = d0;
<<<<<<< HEAD
				d0 <<= left;
=======
				d0 >>= right;
				dst++;
>>>>>>> 671a46baf1b... some performance improvements
				n -= bits - dst_idx;
			} else {
				// 2 source words
				d1 = FB_READL(src++);
				d1 = fb_rev_pixels_in_long(d1, bswapmask);

<<<<<<< HEAD
				d0 = d0 >> right | d1 << left;
=======
				d0 = d0<<left | d1>>right;
				dst++;
>>>>>>> 671a46baf1b... some performance improvements
				n -= bits - dst_idx;
			}
			d0 = fb_rev_pixels_in_long(d0, bswapmask);
			FB_WRITEL(comp(d0, FB_READL(dst), first), dst);
			d0 = d1;
<<<<<<< HEAD
			dst++;
=======
>>>>>>> 671a46baf1b... some performance improvements

			// Main chunk
			m = n % bits;
			n /= bits;
			while ((n >= 4) && !bswapmask) {
				d1 = FB_READL(src++);
<<<<<<< HEAD
				FB_WRITEL(d0 >> right | d1 << left, dst++);
				d0 = d1;
				d1 = FB_READL(src++);
				FB_WRITEL(d0 >> right | d1 << left, dst++);
				d0 = d1;
				d1 = FB_READL(src++);
				FB_WRITEL(d0 >> right | d1 << left, dst++);
				d0 = d1;
				d1 = FB_READL(src++);
				FB_WRITEL(d0 >> right | d1 << left, dst++);
=======
				FB_WRITEL(d0 << left | d1 >> right, dst++);
				d0 = d1;
				d1 = FB_READL(src++);
				FB_WRITEL(d0 << left | d1 >> right, dst++);
				d0 = d1;
				d1 = FB_READL(src++);
				FB_WRITEL(d0 << left | d1 >> right, dst++);
				d0 = d1;
				d1 = FB_READL(src++);
				FB_WRITEL(d0 << left | d1 >> right, dst++);
>>>>>>> 671a46baf1b... some performance improvements
				d0 = d1;
				n -= 4;
			}
			while (n--) {
				d1 = FB_READL(src++);
				d1 = fb_rev_pixels_in_long(d1, bswapmask);
<<<<<<< HEAD
				d0 = d0 >> right | d1 << left;
=======
				d0 = d0 << left | d1 >> right;
>>>>>>> 671a46baf1b... some performance improvements
				d0 = fb_rev_pixels_in_long(d0, bswapmask);
				FB_WRITEL(d0, dst++);
				d0 = d1;
			}

			// Trailing bits
<<<<<<< HEAD
			if (m) {
				if (m <= bits - right) {
					// Single source word
					d0 >>= right;
=======
			if (last) {
				if (m <= right) {
					// Single source word
					d0 <<= left;
>>>>>>> 671a46baf1b... some performance improvements
				} else {
					// 2 source words
					d1 = FB_READL(src);
					d1 = fb_rev_pixels_in_long(d1,
								bswapmask);
<<<<<<< HEAD
					d0 = d0 >> right | d1 << left;
=======
					d0 = d0<<left | d1>>right;
>>>>>>> 671a46baf1b... some performance improvements
				}
				d0 = fb_rev_pixels_in_long(d0, bswapmask);
				FB_WRITEL(comp(d0, FB_READL(dst), last), dst);
			}
		}
	}
}

    /*
     *  Generic bitwise copy algorithm, operating backward
     */

static void
<<<<<<< HEAD
bitcpy_rev(struct fb_info *p, unsigned long __iomem *dst, unsigned dst_idx,
		const unsigned long __iomem *src, unsigned src_idx, int bits,
=======
bitcpy_rev(struct fb_info *p, unsigned long __iomem *dst, int dst_idx,
		const unsigned long __iomem *src, int src_idx, int bits,
>>>>>>> 671a46baf1b... some performance improvements
		unsigned n, u32 bswapmask)
{
	unsigned long first, last;
	int shift;

<<<<<<< HEAD
#if 0
	/*
	 * If you suspect bug in this function, compare it with this simple
	 * memmove implementation.
	 */
	fb_memmove((char *)dst + ((dst_idx & (bits - 1))) / 8,
		   (char *)src + ((src_idx & (bits - 1))) / 8, n / 8);
	return;
#endif

	dst += (dst_idx + n - 1) / bits;
	src += (src_idx + n - 1) / bits;
	dst_idx = (dst_idx + n - 1) % bits;
	src_idx = (src_idx + n - 1) % bits;

	shift = dst_idx-src_idx;

	first = ~fb_shifted_pixels_mask_long(p, (dst_idx + 1) % bits, bswapmask);
	last = fb_shifted_pixels_mask_long(p, (bits + dst_idx + 1 - n) % bits, bswapmask);
=======
	dst += (n-1)/bits;
	src += (n-1)/bits;
	if ((n-1) % bits) {
		dst_idx += (n-1) % bits;
		dst += dst_idx >> (ffs(bits) - 1);
		dst_idx &= bits - 1;
		src_idx += (n-1) % bits;
		src += src_idx >> (ffs(bits) - 1);
		src_idx &= bits - 1;
	}

	shift = dst_idx-src_idx;

	first = fb_shifted_pixels_mask_long(p, bits - 1 - dst_idx, bswapmask);
	last = ~fb_shifted_pixels_mask_long(p, bits - 1 - ((dst_idx-n) % bits),
					    bswapmask);
>>>>>>> 671a46baf1b... some performance improvements

	if (!shift) {
		// Same alignment for source and dest

		if ((unsigned long)dst_idx+1 >= n) {
			// Single word
<<<<<<< HEAD
			if (first)
				last &= first;
			FB_WRITEL( comp( FB_READL(src), FB_READL(dst), last), dst);
=======
			if (last)
				first &= last;
			FB_WRITEL( comp( FB_READL(src), FB_READL(dst), first), dst);
>>>>>>> 671a46baf1b... some performance improvements
		} else {
			// Multiple destination words

			// Leading bits
<<<<<<< HEAD
			if (first) {
=======
			if (first != ~0UL) {
>>>>>>> 671a46baf1b... some performance improvements
				FB_WRITEL( comp( FB_READL(src), FB_READL(dst), first), dst);
				dst--;
				src--;
				n -= dst_idx+1;
			}

			// Main chunk
			n /= bits;
			while (n >= 8) {
				FB_WRITEL(FB_READL(src--), dst--);
				FB_WRITEL(FB_READL(src--), dst--);
				FB_WRITEL(FB_READL(src--), dst--);
				FB_WRITEL(FB_READL(src--), dst--);
				FB_WRITEL(FB_READL(src--), dst--);
				FB_WRITEL(FB_READL(src--), dst--);
				FB_WRITEL(FB_READL(src--), dst--);
				FB_WRITEL(FB_READL(src--), dst--);
				n -= 8;
			}
			while (n--)
				FB_WRITEL(FB_READL(src--), dst--);

			// Trailing bits
<<<<<<< HEAD
			if (last != -1UL)
=======
			if (last)
>>>>>>> 671a46baf1b... some performance improvements
				FB_WRITEL( comp( FB_READL(src), FB_READL(dst), last), dst);
		}
	} else {
		// Different alignment for source and dest
		unsigned long d0, d1;
		int m;

<<<<<<< HEAD
		int const left = shift & (bits-1);
		int const right = -shift & (bits-1);

		if ((unsigned long)dst_idx+1 >= n) {
			// Single destination word
			if (first)
				last &= first;
			d0 = FB_READL(src);
			if (shift < 0) {
				// Single source word
				d0 >>= right;
			} else if (1+(unsigned long)src_idx >= n) {
				// Single source word
				d0 <<= left;
=======
		int const left = -shift & (bits-1);
		int const right = shift & (bits-1);
		bswapmask &= shift;

		if ((unsigned long)dst_idx+1 >= n) {
			// Single destination word
			if (last)
				first &= last;
			d0 = FB_READL(src);
			if (shift < 0) {
				// Single source word
				d0 <<= left;
			} else if (1+(unsigned long)src_idx >= n) {
				// Single source word
				d0 >>= right;
>>>>>>> 671a46baf1b... some performance improvements
			} else {
				// 2 source words
				d1 = FB_READL(src - 1);
				d1 = fb_rev_pixels_in_long(d1, bswapmask);
<<<<<<< HEAD
				d0 = d0 << left | d1 >> right;
			}
			d0 = fb_rev_pixels_in_long(d0, bswapmask);
			FB_WRITEL(comp(d0, FB_READL(dst), last), dst);
=======
				d0 = d0>>right | d1<<left;
			}
			d0 = fb_rev_pixels_in_long(d0, bswapmask);
			FB_WRITEL(comp(d0, FB_READL(dst), first), dst);
>>>>>>> 671a46baf1b... some performance improvements
		} else {
			// Multiple destination words
			/** We must always remember the last value read, because in case
			SRC and DST overlap bitwise (e.g. when moving just one pixel in
			1bpp), we always collect one full long for DST and that might
			overlap with the current long from SRC. We store this value in
			'd0'. */

			d0 = FB_READL(src--);
			d0 = fb_rev_pixels_in_long(d0, bswapmask);
			// Leading bits
			if (shift < 0) {
				// Single source word
				d1 = d0;
<<<<<<< HEAD
				d0 >>= right;
=======
				d0 <<= left;
>>>>>>> 671a46baf1b... some performance improvements
			} else {
				// 2 source words
				d1 = FB_READL(src--);
				d1 = fb_rev_pixels_in_long(d1, bswapmask);
<<<<<<< HEAD
				d0 = d0 << left | d1 >> right;
=======
				d0 = d0>>right | d1<<left;
>>>>>>> 671a46baf1b... some performance improvements
			}
			d0 = fb_rev_pixels_in_long(d0, bswapmask);
			FB_WRITEL(comp(d0, FB_READL(dst), first), dst);
			d0 = d1;
			dst--;
			n -= dst_idx+1;

			// Main chunk
			m = n % bits;
			n /= bits;
			while ((n >= 4) && !bswapmask) {
				d1 = FB_READL(src--);
<<<<<<< HEAD
				FB_WRITEL(d0 << left | d1 >> right, dst--);
				d0 = d1;
				d1 = FB_READL(src--);
				FB_WRITEL(d0 << left | d1 >> right, dst--);
				d0 = d1;
				d1 = FB_READL(src--);
				FB_WRITEL(d0 << left | d1 >> right, dst--);
				d0 = d1;
				d1 = FB_READL(src--);
				FB_WRITEL(d0 << left | d1 >> right, dst--);
=======
				FB_WRITEL(d0 >> right | d1 << left, dst--);
				d0 = d1;
				d1 = FB_READL(src--);
				FB_WRITEL(d0 >> right | d1 << left, dst--);
				d0 = d1;
				d1 = FB_READL(src--);
				FB_WRITEL(d0 >> right | d1 << left, dst--);
				d0 = d1;
				d1 = FB_READL(src--);
				FB_WRITEL(d0 >> right | d1 << left, dst--);
>>>>>>> 671a46baf1b... some performance improvements
				d0 = d1;
				n -= 4;
			}
			while (n--) {
				d1 = FB_READL(src--);
				d1 = fb_rev_pixels_in_long(d1, bswapmask);
<<<<<<< HEAD
				d0 = d0 << left | d1 >> right;
=======
				d0 = d0 >> right | d1 << left;
>>>>>>> 671a46baf1b... some performance improvements
				d0 = fb_rev_pixels_in_long(d0, bswapmask);
				FB_WRITEL(d0, dst--);
				d0 = d1;
			}

			// Trailing bits
<<<<<<< HEAD
			if (m) {
				if (m <= bits - left) {
					// Single source word
					d0 <<= left;
=======
			if (last) {
				if (m <= left) {
					// Single source word
					d0 >>= right;
>>>>>>> 671a46baf1b... some performance improvements
				} else {
					// 2 source words
					d1 = FB_READL(src);
					d1 = fb_rev_pixels_in_long(d1,
								bswapmask);
<<<<<<< HEAD
					d0 = d0 << left | d1 >> right;
=======
					d0 = d0>>right | d1<<left;
>>>>>>> 671a46baf1b... some performance improvements
				}
				d0 = fb_rev_pixels_in_long(d0, bswapmask);
				FB_WRITEL(comp(d0, FB_READL(dst), last), dst);
			}
		}
	}
}

void cfb_copyarea(struct fb_info *p, const struct fb_copyarea *area)
{
	u32 dx = area->dx, dy = area->dy, sx = area->sx, sy = area->sy;
	u32 height = area->height, width = area->width;
	unsigned long const bits_per_line = p->fix.line_length*8u;
<<<<<<< HEAD
	unsigned long __iomem *base = NULL;
	int bits = BITS_PER_LONG, bytes = bits >> 3;
	unsigned dst_idx = 0, src_idx = 0, rev_copy = 0;
=======
	unsigned long __iomem *dst = NULL, *src = NULL;
	int bits = BITS_PER_LONG, bytes = bits >> 3;
	int dst_idx = 0, src_idx = 0, rev_copy = 0;
>>>>>>> 671a46baf1b... some performance improvements
	u32 bswapmask = fb_compute_bswapmask(p);

	if (p->state != FBINFO_STATE_RUNNING)
		return;

	/* if the beginning of the target area might overlap with the end of
	the source area, be have to copy the area reverse. */
	if ((dy == sy && dx > sx) || (dy > sy)) {
		dy += height;
		sy += height;
		rev_copy = 1;
	}

	// split the base of the framebuffer into a long-aligned address and the
	// index of the first bit
<<<<<<< HEAD
	base = (unsigned long __iomem *)((unsigned long)p->screen_base & ~(bytes-1));
=======
	dst = src = (unsigned long __iomem *)((unsigned long)p->screen_base & ~(bytes-1));
>>>>>>> 671a46baf1b... some performance improvements
	dst_idx = src_idx = 8*((unsigned long)p->screen_base & (bytes-1));
	// add offset of source and target area
	dst_idx += dy*bits_per_line + dx*p->var.bits_per_pixel;
	src_idx += sy*bits_per_line + sx*p->var.bits_per_pixel;

	if (p->fbops->fb_sync)
		p->fbops->fb_sync(p);

	if (rev_copy) {
		while (height--) {
			dst_idx -= bits_per_line;
			src_idx -= bits_per_line;
<<<<<<< HEAD
			bitcpy_rev(p, base + (dst_idx / bits), dst_idx % bits,
				base + (src_idx / bits), src_idx % bits, bits,
=======
			dst += dst_idx >> (ffs(bits) - 1);
			dst_idx &= (bytes - 1);
			src += src_idx >> (ffs(bits) - 1);
			src_idx &= (bytes - 1);
			bitcpy_rev(p, dst, dst_idx, src, src_idx, bits,
>>>>>>> 671a46baf1b... some performance improvements
				width*p->var.bits_per_pixel, bswapmask);
		}
	} else {
		while (height--) {
<<<<<<< HEAD
			bitcpy(p, base + (dst_idx / bits), dst_idx % bits,
				base + (src_idx / bits), src_idx % bits, bits,
=======
			dst += dst_idx >> (ffs(bits) - 1);
			dst_idx &= (bytes - 1);
			src += src_idx >> (ffs(bits) - 1);
			src_idx &= (bytes - 1);
			bitcpy(p, dst, dst_idx, src, src_idx, bits,
>>>>>>> 671a46baf1b... some performance improvements
				width*p->var.bits_per_pixel, bswapmask);
			dst_idx += bits_per_line;
			src_idx += bits_per_line;
		}
	}
}

EXPORT_SYMBOL(cfb_copyarea);

MODULE_AUTHOR("James Simmons <jsimmons@users.sf.net>");
MODULE_DESCRIPTION("Generic software accelerated copyarea");
MODULE_LICENSE("GPL");


/*
 * symlink.c
 *
 * PURPOSE
 *	Symlink handling routines for the OSTA-UDF(tm) filesystem.
 *
 * COPYRIGHT
 *	This file is distributed under the terms of the GNU General Public
 *	License (GPL). Copies of the GPL can be obtained from:
 *		ftp://prep.ai.mit.edu/pub/gnu/GPL
 *	Each contributing author retains all rights to their own work.
 *
 *  (C) 1998-2001 Ben Fennema
 *  (C) 1999 Stelias Computing Inc
 *
 * HISTORY
 *
 *  04/16/99 blf  Created.
 *
 */

#include "udfdecl.h"
#include <asm/uaccess.h>
#include <linux/errno.h>
#include <linux/fs.h>
#include <linux/time.h>
#include <linux/mm.h>
#include <linux/stat.h>
#include <linux/pagemap.h>
#include <linux/buffer_head.h>
#include "udf_i.h"

<<<<<<< HEAD
<<<<<<< HEAD
static int udf_pc_to_char(struct super_block *sb, unsigned char *from,
			  int fromlen, unsigned char *to, int tolen)
{
	struct pathComponent *pc;
	int elen = 0;
	int comp_len;
	unsigned char *p = to;

	/* Reserve one byte for terminating \0 */
	tolen--;
=======
=======
>>>>>>> master
static void udf_pc_to_char(struct super_block *sb, unsigned char *from,
			   int fromlen, unsigned char *to)
{
	struct pathComponent *pc;
	int elen = 0;
	unsigned char *p = to;

<<<<<<< HEAD
>>>>>>> 671a46baf1b... some performance improvements
=======
>>>>>>> master
	while (elen < fromlen) {
		pc = (struct pathComponent *)(from + elen);
		switch (pc->componentType) {
		case 1:
			/*
			 * Symlink points to some place which should be agreed
 			 * upon between originator and receiver of the media. Ignore.
			 */
			if (pc->lengthComponentIdent > 0)
				break;
			/* Fall through */
		case 2:
<<<<<<< HEAD
<<<<<<< HEAD
			if (tolen == 0)
				return -ENAMETOOLONG;
			p = to;
			*p++ = '/';
			tolen--;
			break;
		case 3:
			if (tolen < 3)
				return -ENAMETOOLONG;
			memcpy(p, "../", 3);
			p += 3;
			tolen -= 3;
			break;
		case 4:
			if (tolen < 2)
				return -ENAMETOOLONG;
			memcpy(p, "./", 2);
			p += 2;
			tolen -= 2;
			/* that would be . - just ignore */
			break;
		case 5:
			comp_len = udf_get_filename(sb, pc->componentIdent,
						    pc->lengthComponentIdent,
						    p, tolen);
			p += comp_len;
			tolen -= comp_len;
			if (tolen == 0)
				return -ENAMETOOLONG;
			*p++ = '/';
			tolen--;
=======
=======
>>>>>>> master
			p = to;
			*p++ = '/';
			break;
		case 3:
			memcpy(p, "../", 3);
			p += 3;
			break;
		case 4:
			memcpy(p, "./", 2);
			p += 2;
			/* that would be . - just ignore */
			break;
		case 5:
			p += udf_get_filename(sb, pc->componentIdent, p,
					      pc->lengthComponentIdent);
			*p++ = '/';
<<<<<<< HEAD
>>>>>>> 671a46baf1b... some performance improvements
=======
>>>>>>> master
			break;
		}
		elen += sizeof(struct pathComponent) + pc->lengthComponentIdent;
	}
	if (p > to + 1)
		p[-1] = '\0';
	else
		p[0] = '\0';
<<<<<<< HEAD
<<<<<<< HEAD
	return 0;
=======
>>>>>>> 671a46baf1b... some performance improvements
=======
>>>>>>> master
}

static int udf_symlink_filler(struct file *file, struct page *page)
{
	struct inode *inode = page->mapping->host;
	struct buffer_head *bh = NULL;
	unsigned char *symlink;
<<<<<<< HEAD
<<<<<<< HEAD
	int err;
=======
	int err = -EIO;
>>>>>>> 671a46baf1b... some performance improvements
=======
	int err = -EIO;
>>>>>>> master
	unsigned char *p = kmap(page);
	struct udf_inode_info *iinfo;
	uint32_t pos;

<<<<<<< HEAD
<<<<<<< HEAD
	/* We don't support symlinks longer than one block */
	if (inode->i_size > inode->i_sb->s_blocksize) {
		err = -ENAMETOOLONG;
		goto out_unmap;
	}

=======
>>>>>>> 671a46baf1b... some performance improvements
=======
>>>>>>> master
	iinfo = UDF_I(inode);
	pos = udf_block_map(inode, 0);

	down_read(&iinfo->i_data_sem);
	if (iinfo->i_alloc_type == ICBTAG_FLAG_AD_IN_ICB) {
		symlink = iinfo->i_ext.i_data + iinfo->i_lenEAttr;
	} else {
		bh = sb_bread(inode->i_sb, pos);

<<<<<<< HEAD
<<<<<<< HEAD
		if (!bh) {
			err = -EIO;
			goto out_unlock_inode;
		}
=======
		if (!bh)
			goto out;
>>>>>>> 671a46baf1b... some performance improvements
=======
		if (!bh)
			goto out;
>>>>>>> master

		symlink = bh->b_data;
	}

<<<<<<< HEAD
<<<<<<< HEAD
	err = udf_pc_to_char(inode->i_sb, symlink, inode->i_size, p, PAGE_SIZE);
	brelse(bh);
	if (err)
		goto out_unlock_inode;
=======
	udf_pc_to_char(inode->i_sb, symlink, inode->i_size, p);
	brelse(bh);
>>>>>>> 671a46baf1b... some performance improvements
=======
	udf_pc_to_char(inode->i_sb, symlink, inode->i_size, p);
	brelse(bh);
>>>>>>> master

	up_read(&iinfo->i_data_sem);
	SetPageUptodate(page);
	kunmap(page);
	unlock_page(page);
	return 0;

<<<<<<< HEAD
<<<<<<< HEAD
out_unlock_inode:
	up_read(&iinfo->i_data_sem);
	SetPageError(page);
out_unmap:
=======
out:
	up_read(&iinfo->i_data_sem);
	SetPageError(page);
>>>>>>> 671a46baf1b... some performance improvements
=======
out:
	up_read(&iinfo->i_data_sem);
	SetPageError(page);
>>>>>>> master
	kunmap(page);
	unlock_page(page);
	return err;
}

/*
 * symlinks can't do much...
 */
const struct address_space_operations udf_symlink_aops = {
	.readpage		= udf_symlink_filler,
};

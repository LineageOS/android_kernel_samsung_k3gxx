/*
 * fs/sdcardfs/super.c
 *
 * Copyright (c) 2013 Samsung Electronics Co. Ltd
<<<<<<< HEAD
 *   Authors: Daeho Jeong, Woojoong Lee, Seunghwan Hyun,
 *               Sunghwan Yun, Sungjong Seo
 *
 * This program has been developed as a stackable file system based on
 * the WrapFS which written by
=======
 *   Authors: Daeho Jeong, Woojoong Lee, Seunghwan Hyun, 
 *               Sunghwan Yun, Sungjong Seo
 *                      
 * This program has been developed as a stackable file system based on
 * the WrapFS which written by 
>>>>>>> 671a46baf1b... some performance improvements
 *
 * Copyright (c) 1998-2011 Erez Zadok
 * Copyright (c) 2009     Shrikar Archak
 * Copyright (c) 2003-2011 Stony Brook University
 * Copyright (c) 2003-2011 The Research Foundation of SUNY
 *
 * This file is dual licensed.  It may be redistributed and/or modified
 * under the terms of the Apache 2.0 License OR version 2 of the GNU
 * General Public License.
 */

#include "sdcardfs.h"

/*
 * The inode cache is used with alloc_inode for both our inode info and the
 * vfs inode.
 */
static struct kmem_cache *sdcardfs_inode_cachep;

<<<<<<< HEAD
/*
 * To support the top references, we must track some data separately.
 * An sdcardfs_inode_info always has a reference to its data, and once set up,
 * also has a reference to its top. The top may be itself, in which case it
 * holds two references to its data. When top is changed, it takes a ref to the
 * new data and then drops the ref to the old data.
 */
static struct kmem_cache *sdcardfs_inode_data_cachep;

void data_release(struct kref *ref)
{
	struct sdcardfs_inode_data *data =
		container_of(ref, struct sdcardfs_inode_data, refcount);

	kmem_cache_free(sdcardfs_inode_data_cachep, data);
}

=======
>>>>>>> 671a46baf1b... some performance improvements
/* final actions when unmounting a file system */
static void sdcardfs_put_super(struct super_block *sb)
{
	struct sdcardfs_sb_info *spd;
	struct super_block *s;

	spd = SDCARDFS_SB(sb);
	if (!spd)
		return;

<<<<<<< HEAD
	if (spd->obbpath_s) {
=======
	printk(KERN_ERR "sdcardfs: umounted dev_name %s\n", 
				spd->devpath ? spd->devpath : "");
	if(spd->devpath)
		kfree(spd->devpath);

	if(spd->obbpath_s) {
>>>>>>> 671a46baf1b... some performance improvements
		kfree(spd->obbpath_s);
		path_put(&spd->obbpath);
	}

<<<<<<< HEAD
=======
	if(spd->options.label)
		kfree(spd->options.label);

>>>>>>> 671a46baf1b... some performance improvements
	/* decrement lower super references */
	s = sdcardfs_lower_super(sb);
	sdcardfs_set_lower_super(sb, NULL);
	atomic_dec(&s->s_active);

<<<<<<< HEAD
=======
	if(spd->pkgl_id)
		packagelist_destroy(spd->pkgl_id);

>>>>>>> 671a46baf1b... some performance improvements
	kfree(spd);
	sb->s_fs_info = NULL;
}

static int sdcardfs_statfs(struct dentry *dentry, struct kstatfs *buf)
{
	int err;
	struct path lower_path;
	u32 min_blocks;
	struct sdcardfs_sb_info *sbi = SDCARDFS_SB(dentry->d_sb);

	sdcardfs_get_lower_path(dentry, &lower_path);
	err = vfs_statfs(&lower_path, buf);
	sdcardfs_put_lower_path(dentry, &lower_path);

	if (sbi->options.reserved_mb) {
		/* Invalid statfs informations. */
		if (buf->f_bsize == 0) {
<<<<<<< HEAD
			pr_err("Returned block size is zero.\n");
			return -EINVAL;
		}

		min_blocks = ((sbi->options.reserved_mb * 1024 * 1024)/buf->f_bsize);
		buf->f_blocks -= min_blocks;

=======
			printk(KERN_ERR "Returned block size is zero.\n");
			return -EINVAL;
		}
	
		min_blocks = ((sbi->options.reserved_mb * 1024 * 1024)/buf->f_bsize);
		buf->f_blocks -= min_blocks;
	
>>>>>>> 671a46baf1b... some performance improvements
		if (buf->f_bavail > min_blocks)
			buf->f_bavail -= min_blocks;
		else
			buf->f_bavail = 0;
<<<<<<< HEAD

=======
	
>>>>>>> 671a46baf1b... some performance improvements
		/* Make reserved blocks invisiable to media storage */
		buf->f_bfree = buf->f_bavail;
	}

	/* set return buf to our f/s to avoid confusing user-level utils */
	buf->f_type = SDCARDFS_SUPER_MAGIC;

	return err;
}

/*
 * @flags: numeric mount options
 * @options: mount options string
 */
static int sdcardfs_remount_fs(struct super_block *sb, int *flags, char *options)
{
	int err = 0;

	/*
	 * The VFS will take care of "ro" and "rw" flags among others.  We
	 * can safely accept a few flags (RDONLY, MANDLOCK), and honor
	 * SILENT, but anything else left over is an error.
	 */
	if ((*flags & ~(MS_RDONLY | MS_MANDLOCK | MS_SILENT)) != 0) {
<<<<<<< HEAD
		pr_err("sdcardfs: remount flags 0x%x unsupported\n", *flags);
=======
		printk(KERN_ERR
		       "sdcardfs: remount flags 0x%x unsupported\n", *flags);
>>>>>>> 671a46baf1b... some performance improvements
		err = -EINVAL;
	}

	return err;
}

/*
<<<<<<< HEAD
 * @mnt: mount point we are remounting
 * @sb: superblock we are remounting
 * @flags: numeric mount options
 * @options: mount options string
 */
static int sdcardfs_remount_fs2(struct vfsmount *mnt, struct super_block *sb,
						int *flags, char *options)
{
	int err = 0;

	/*
	 * The VFS will take care of "ro" and "rw" flags among others.  We
	 * can safely accept a few flags (RDONLY, MANDLOCK), and honor
	 * SILENT, but anything else left over is an error.
	 */
	if ((*flags & ~(MS_RDONLY | MS_MANDLOCK | MS_SILENT | MS_REMOUNT)) != 0) {
		pr_err("sdcardfs: remount flags 0x%x unsupported\n", *flags);
		err = -EINVAL;
	}
	pr_info("Remount options were %s for vfsmnt %p.\n", options, mnt);
	err = parse_options_remount(sb, options, *flags & ~MS_SILENT, mnt->data);


	return err;
}

static void *sdcardfs_clone_mnt_data(void *data)
{
	struct sdcardfs_vfsmount_options *opt = kmalloc(sizeof(struct sdcardfs_vfsmount_options), GFP_KERNEL);
	struct sdcardfs_vfsmount_options *old = data;

	if (!opt)
		return NULL;
	opt->gid = old->gid;
	opt->mask = old->mask;
	return opt;
}

static void sdcardfs_copy_mnt_data(void *data, void *newdata)
{
	struct sdcardfs_vfsmount_options *old = data;
	struct sdcardfs_vfsmount_options *new = newdata;

	old->gid = new->gid;
	old->mask = new->mask;
}

/*
=======
>>>>>>> 671a46baf1b... some performance improvements
 * Called by iput() when the inode reference count reached zero
 * and the inode is not hashed anywhere.  Used to clear anything
 * that needs to be, before the inode is completely destroyed and put
 * on the inode free list.
 */
static void sdcardfs_evict_inode(struct inode *inode)
{
	struct inode *lower_inode;

	truncate_inode_pages(&inode->i_data, 0);
<<<<<<< HEAD
	set_top(SDCARDFS_I(inode), NULL);
=======
>>>>>>> 671a46baf1b... some performance improvements
	clear_inode(inode);
	/*
	 * Decrement a reference to a lower_inode, which was incremented
	 * by our read_inode when it was created initially.
	 */
	lower_inode = sdcardfs_lower_inode(inode);
	sdcardfs_set_lower_inode(inode, NULL);
	iput(lower_inode);
}

static struct inode *sdcardfs_alloc_inode(struct super_block *sb)
{
	struct sdcardfs_inode_info *i;
<<<<<<< HEAD
	struct sdcardfs_inode_data *d;
=======
>>>>>>> 671a46baf1b... some performance improvements

	i = kmem_cache_alloc(sdcardfs_inode_cachep, GFP_KERNEL);
	if (!i)
		return NULL;

	/* memset everything up to the inode to 0 */
	memset(i, 0, offsetof(struct sdcardfs_inode_info, vfs_inode));

<<<<<<< HEAD
	d = kmem_cache_alloc(sdcardfs_inode_data_cachep,
					GFP_KERNEL | __GFP_ZERO);
	if (!d) {
		kmem_cache_free(sdcardfs_inode_cachep, i);
		return NULL;
	}

	i->data = d;
	kref_init(&d->refcount);

=======
>>>>>>> 671a46baf1b... some performance improvements
	i->vfs_inode.i_version = 1;
	return &i->vfs_inode;
}

<<<<<<< HEAD
static void i_callback(struct rcu_head *head)
{
	struct inode *inode = container_of(head, struct inode, i_rcu);

	release_own_data(SDCARDFS_I(inode));
	kmem_cache_free(sdcardfs_inode_cachep, SDCARDFS_I(inode));
}

static void sdcardfs_destroy_inode(struct inode *inode)
{
	call_rcu(&inode->i_rcu, i_callback);
=======
static void sdcardfs_destroy_inode(struct inode *inode)
{
	kmem_cache_free(sdcardfs_inode_cachep, SDCARDFS_I(inode));
>>>>>>> 671a46baf1b... some performance improvements
}

/* sdcardfs inode cache constructor */
static void init_once(void *obj)
{
	struct sdcardfs_inode_info *i = obj;

	inode_init_once(&i->vfs_inode);
}

int sdcardfs_init_inode_cache(void)
{
<<<<<<< HEAD
=======
	int err = 0;

>>>>>>> 671a46baf1b... some performance improvements
	sdcardfs_inode_cachep =
		kmem_cache_create("sdcardfs_inode_cache",
				  sizeof(struct sdcardfs_inode_info), 0,
				  SLAB_RECLAIM_ACCOUNT, init_once);
<<<<<<< HEAD

	if (!sdcardfs_inode_cachep)
		return -ENOMEM;

	sdcardfs_inode_data_cachep =
		kmem_cache_create("sdcardfs_inode_data_cache",
				  sizeof(struct sdcardfs_inode_data), 0,
				  SLAB_RECLAIM_ACCOUNT, NULL);
	if (!sdcardfs_inode_data_cachep) {
		kmem_cache_destroy(sdcardfs_inode_cachep);
		return -ENOMEM;
	}

	return 0;
=======
	if (!sdcardfs_inode_cachep)
		err = -ENOMEM;
	return err;
>>>>>>> 671a46baf1b... some performance improvements
}

/* sdcardfs inode cache destructor */
void sdcardfs_destroy_inode_cache(void)
{
<<<<<<< HEAD
	kmem_cache_destroy(sdcardfs_inode_data_cachep);
	kmem_cache_destroy(sdcardfs_inode_cachep);
=======
	if (sdcardfs_inode_cachep)
		kmem_cache_destroy(sdcardfs_inode_cachep);
}

static long sdcardfs_propagate_lookup(struct super_block *sb, char* pathname) {
	long ret = 0;
	char *propagate_path = NULL;
	struct sdcardfs_sb_info *sbi;
	struct path sibling_path;
	const struct cred *saved_cred = NULL;

	sbi = SDCARDFS_SB(sb);
	propagate_path = kmalloc(PATH_MAX, GFP_KERNEL);
	OVERRIDE_ROOT_CRED(saved_cred);
	if (sbi->options.type != TYPE_NONE && sbi->options.type != TYPE_DEFAULT) {
		snprintf(propagate_path, PATH_MAX, "/mnt/runtime/default/%s%s",
				sbi->options.label, pathname);
		ret = (long)kern_path(propagate_path, LOOKUP_FOLLOW, &sibling_path);
		if (!ret)
			path_put(&sibling_path);
	}

	if (sbi->options.type != TYPE_NONE && sbi->options.type != TYPE_READ) {
		snprintf(propagate_path, PATH_MAX, "/mnt/runtime/read/%s%s",
				sbi->options.label, pathname);
		ret = (long)kern_path(propagate_path, LOOKUP_FOLLOW, &sibling_path);
		if (!ret)
			path_put(&sibling_path);
	}

	if (sbi->options.type != TYPE_NONE && sbi->options.type != TYPE_WRITE) {
		snprintf(propagate_path, PATH_MAX, "/mnt/runtime/write/%s%s",
				sbi->options.label, pathname);
		ret = (long)kern_path(propagate_path, LOOKUP_FOLLOW, &sibling_path);
		if (!ret)
			path_put(&sibling_path);
	}

	if (sbi->options.type != TYPE_NONE) {
		snprintf(propagate_path, PATH_MAX, "/storage/%s%s",
				sbi->options.label, pathname);
		ret = (long)kern_path(propagate_path, LOOKUP_FOLLOW, &sibling_path);
		if (!ret)
			path_put(&sibling_path);
	}
	REVERT_CRED(saved_cred);
	kfree(propagate_path);
	return ret;
>>>>>>> 671a46baf1b... some performance improvements
}

/*
 * Used only in nfs, to kill any pending RPC tasks, so that subsequent
 * code can actually succeed and won't leave tasks that need handling.
 */
static void sdcardfs_umount_begin(struct super_block *sb)
{
	struct super_block *lower_sb;

	lower_sb = sdcardfs_lower_super(sb);
	if (lower_sb && lower_sb->s_op && lower_sb->s_op->umount_begin)
		lower_sb->s_op->umount_begin(lower_sb);
}

<<<<<<< HEAD
static int sdcardfs_show_options(struct vfsmount *mnt, struct seq_file *m,
			struct dentry *root)
{
	struct sdcardfs_sb_info *sbi = SDCARDFS_SB(root->d_sb);
	struct sdcardfs_mount_options *opts = &sbi->options;
	struct sdcardfs_vfsmount_options *vfsopts = mnt->data;

	if (opts->fs_low_uid != 0)
		seq_printf(m, ",fsuid=%u", opts->fs_low_uid);
	if (opts->fs_low_gid != 0)
		seq_printf(m, ",fsgid=%u", opts->fs_low_gid);
	if (vfsopts->gid != 0)
		seq_printf(m, ",gid=%u", vfsopts->gid);
	if (opts->multiuser)
		seq_puts(m, ",multiuser");
	if (vfsopts->mask)
		seq_printf(m, ",mask=%u", vfsopts->mask);
	if (opts->fs_user_id)
		seq_printf(m, ",userid=%u", opts->fs_user_id);
=======
static int sdcardfs_show_options(struct seq_file *m, struct dentry *root)
{
	struct sdcardfs_sb_info *sbi = SDCARDFS_SB(root->d_sb);
	struct sdcardfs_mount_options *opts = &sbi->options;

	if (opts->fs_low_uid != 0)
		seq_printf(m, ",low_uid=%u", opts->fs_low_uid);
	if (opts->fs_low_gid != 0)
		seq_printf(m, ",low_gid=%u", opts->fs_low_gid);
	if (opts->gid != 0)
		seq_printf(m, ",gid=%u", opts->gid);
	if (opts->userid != 0)
		seq_printf(m, ",userid=%u", opts->userid);
	if (opts->multi_user)
		seq_printf(m, ",multi_user");
	if (opts->mask != 0)
		seq_printf(m, ",mask=%04o", opts->mask);
>>>>>>> 671a46baf1b... some performance improvements
	if (opts->reserved_mb != 0)
		seq_printf(m, ",reserved=%uMB", opts->reserved_mb);

	return 0;
};

const struct super_operations sdcardfs_sops = {
	.put_super	= sdcardfs_put_super,
	.statfs		= sdcardfs_statfs,
	.remount_fs	= sdcardfs_remount_fs,
<<<<<<< HEAD
	.remount_fs2	= sdcardfs_remount_fs2,
	.clone_mnt_data	= sdcardfs_clone_mnt_data,
	.copy_mnt_data	= sdcardfs_copy_mnt_data,
	.evict_inode	= sdcardfs_evict_inode,
	.umount_begin	= sdcardfs_umount_begin,
	.show_options2	= sdcardfs_show_options,
	.alloc_inode	= sdcardfs_alloc_inode,
	.destroy_inode	= sdcardfs_destroy_inode,
	.drop_inode	= generic_delete_inode,
=======
	.evict_inode	= sdcardfs_evict_inode,
	.umount_begin	= sdcardfs_umount_begin,
	.show_options	= sdcardfs_show_options,
	.alloc_inode	= sdcardfs_alloc_inode,
	.destroy_inode	= sdcardfs_destroy_inode,
	.drop_inode	= generic_delete_inode,
};

const struct super_operations sdcardfs_multimount_sops = {
	.put_super	= sdcardfs_put_super,
	.statfs		= sdcardfs_statfs,
	.remount_fs	= sdcardfs_remount_fs,
	.evict_inode	= sdcardfs_evict_inode,
	.umount_begin	= sdcardfs_umount_begin,
	.show_options	= sdcardfs_show_options,
	.alloc_inode	= sdcardfs_alloc_inode,
	.destroy_inode	= sdcardfs_destroy_inode,
	.drop_inode	= generic_delete_inode,
	.unlink_callback = sdcardfs_propagate_lookup,
>>>>>>> 671a46baf1b... some performance improvements
};

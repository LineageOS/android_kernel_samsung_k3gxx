#ifndef _LINEAR_H
#define _LINEAR_H

struct dev_info {
	struct md_rdev	*rdev;
	sector_t	end_sector;
};

struct linear_conf
{
	struct rcu_head		rcu;
	sector_t		array_sectors;
<<<<<<< HEAD
<<<<<<< HEAD
	int			raid_disks; /* a copy of mddev->raid_disks */
=======
>>>>>>> 671a46baf1b... some performance improvements
=======
>>>>>>> master
	struct dev_info		disks[0];
};
#endif

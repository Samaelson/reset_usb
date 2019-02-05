/*
 * This is a Linux command-line tool to reset a usb device
 * based on Vendor ID and Product ID
 *
 * Based upon:
 * reset_usb.c by Andreas Schramm.  Copyright 2019.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; see the file LICENSE.txt.  If not, write to
 * the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdbool.h>

#include <libusb-1.0/libusb.h> // requires sudo apt-get install libusb-dev

#define VERSION	"0.2"

static const char *prgname;


static unsigned long unsigned_val (const char *arg, unsigned long max)
{
	unsigned long val;

	errno = 0;
	val = strtoul(arg, NULL, 0);
	if (errno || val > max) {
		fprintf(stderr, "%s: bad value (max=0x%lx)\n", arg, max);
		exit(EINVAL);
	}
	return val;
}


int rst_dev_con(int vid, int pid)
{
    int resetStatus = 0;

    libusb_context * context;

    libusb_init(&context);
    
    libusb_device_handle * dev_handle = libusb_open_device_with_vid_pid(context,vid,pid);
    if (dev_handle == NULL){
      resetStatus = 1;
    }
    else{
      resetStatus = libusb_reset_device(dev_handle);
    }

    libusb_exit(context);

    return resetStatus;
}


/* ------------ Main ------------ */

int main (int argc, char *argv[])
{
	const char *slash;
	int retValue = 0;
	unsigned short usb_vid;
	unsigned short usb_pid;

	prgname = argv[0];
	slash = strrchr(prgname, '/');
	if (slash)
		prgname = slash + 1;

	printf("\n%s: version %s\n", prgname, VERSION);
	printf("Reset USB by Andreas Schramm\n\n");

	if (argc < 3) {
		printf("\nUsage:  %s <arg1> <arg2>\n", prgname);
		printf("\nwhere the argumnets mus be: \n");
		printf("			 <arg1>   # (vendor id of device to be resetted, eg. 0x0403)");
		printf("			 <arg2>   # (product id of device to be resetted, eg. 0x6001)");
		exit(0);
	}

	usb_vid = unsigned_val(argv[1], 0xffff);
	usb_pid = unsigned_val(argv[2], 0xffff);

	retValue = rst_dev_con(usb_vid, usb_pid);
	if(retValue == 1){
		printf("Reseting USB device vid_%04x&pid_%04x failed!\n",usb_vid,usb_pid);
		return 1;
	}

	printf("Reseting USB device vid_%04x&pid_%04x successful!\n",usb_vid,usb_pid);

	return 0;
}

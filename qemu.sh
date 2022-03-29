#!/bin/sh
set -e
. ./generateISO.sh

qemu-system-$(./target-triplet-to-arch.sh $HOST) -cdrom LegacyOS.iso

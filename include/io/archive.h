#ifndef EZI_IO_ARCHIVE_H
#define EZI_IO_ARCHIVE_H

#define EZI_ARCHIVE_BLOCK_SIZE 0x1000

#include "ds/slist.h"

int
ezi_archive_list_contents(const char *filename, struct ezi_slist *outlist);

int
ezi_archive_extract(const char *filename);

#endif /* EZI_IO_ARCHIVE_H */

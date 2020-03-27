#ifndef EZI_DATA_SOFTWARE_SOURCE_H
#define EZI_DATA_SOFTWARE_SOURCE_H

#include "ds/hash_table.h"

#include <stdint.h>
#include <stdio.h>

struct ezi_software_source
{
    FILE *source_fp;
    struct
    {
        uint32_t version;
        uint32_t checksum;
        uint64_t last_update;
        uint32_t count;
    } metadata;
};

/*!
 * \breif Initializes a software database source to use file streams
 *
 * This function simply opens two read/write files from the installed,
 * and available software respectively
 *
 * \param [out]  src       A pointer to the source object to initialize
 * \param [in]   filename  The filename of the software sources
 *
 * \return 0 for success, -1 for otherwise with errno set
 */
int
init_ezi_software_source_fs(struct ezi_software_source *src,
                            const char *                filename,
                            uint32_t                    version);

/*!
 * \breif Initializes a software database source to use memory streams
 *
 * This function simply opens two read/write files from the installed,
 * and available software respectively
 *
 * \param [out]  src       A pointer to the source object to initialize
 * \param [in]   buf       A pointer to the buffer to use as a memory stream
 * \param [in]   len       The length of the buffer
 *
 * \return 0 for success, -1 for otherwise with errno set
 */
int
init_ezi_software_source_ms(struct ezi_software_source *src,
                            void *                      buf,
                            size_t                      len,
                            uint32_t                    version);

/*!
 * \brief Loads data source data from a file
 *
 * \param [in]  src  The source to load the data from
 * \param [out] ht   The source hashtable into which to store the loaded data
 *
 * \return 0 for success, -1 for otherwise with errno set
 */
int
ezi_software_source_load(struct ezi_software_source *src,
                         struct ezi_hash_table *     ht);

/*!
 * \brief Saves data source data into a hashtable
 *
 * \param [in]  src  The source to save the data from
 * \param [out] ht   The destination hashtable into which to store the data
 *
 * \return 0 for success, -1 for otherwise with errno set
 */
int
ezi_software_source_save(struct ezi_software_source *src,
                         struct ezi_hash_table *     ht);

/*!
 * \brief Frees all used resources by a software source pointed to by \see src
 *
 * \param [in,out] src  A pointer to the source to be freed
 */
void
free_ezi_software_source(struct ezi_software_source *src);

#endif /* EZI_DATA_SOFTWARE_SOURCE_H */

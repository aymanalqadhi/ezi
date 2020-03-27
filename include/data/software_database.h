#ifndef EZI_DATA_SOFTWARE_DATABASE_H
#define EZI_DATA_SOFTWARE_DATABASE_H

#include "data/software.h"
#include "data/software_source.h"
#include "ds/hash_table.h"

#include <stdint.h>

struct ezi_software_table
{
    struct ezi_software_source source;
    struct ezi_hash_table      data;
};

struct ezi_software_database
{
    struct ezi_software_table installed;
    struct ezi_software_table available;
    unsigned int              inited : 1;
};

/*!
 * \brief Initializes a software database with a data source
 *
 * \param [in,out] db         A pointer to the database object to be initialized
 * \param [in]     installed  A pointer to the installed table data source
 * \param [in]     available  A pointer to the available table data source
 *
 * \return 0 for success, -1 for error with errno set
 */
int
init_ezi_software_database(struct ezi_software_database *db,
                           struct ezi_software_source *  installed,
                           struct ezi_software_source *  available);

/*!
 * \brief Loads the database from a spcific source
 *
 * \param [in] db  A pointer to the database to load
 * \param [in] src A pointer to the database source
 *
 * \return 0 for success, -1 for error with errno set
 */
int
ezi_software_database_load(struct ezi_software_database *db);

/*!
 * \brief Saves the database into a spcific source
 *
 * \param [in] db  A pointer to the database to load
 *
 * \return 0 for success, -1 for error with errno set
 */
int
ezi_software_database_save(struct ezi_software_database *db);

/*!
 * \brief Adds a software record pointed to by \see sw into a software table
 *        pointed to by \see tbl
 *
 * \param [in,out]  tbl  A pointer to the table into which to insert the record
 * \pram  [in]      sw   A pointer to the record which to add
 *
 * \return 0 on success, -1 otherwise with errno set
 */
int
ezi_software_table_insert(struct ezi_software_table *tbl,
                          const struct ezi_software *sw);

/*!
 * \brief Gets an item which name is specified with \see name from a
 *        software table pointed to by \see tbl
 *
 * \param [in]  tbl   a pointer to the table from which to get the item
 * \param [in]  name  the name of the item to get
 * \param [out] sw    a pointer to the output item if was found
 *
 * \return 0 on success, -1 otherwise with errno set
 */
int
ezi_software_table_get(struct ezi_software_table *tbl,
                       const char *               name,
                       struct ezi_software **     sw);

/*!
 * \brief Removes an item which name is specified with \see name from a
 *        software table pointed to by \see tbl
 *
 * \param [in,out]  tbl   A pointer to the table from which to remove the record
 * \param [in]      name  The name of the item to be removed
 *
 * \return 0 on success, -1 otherwise with errno set
 */
int
ezi_software_table_remove(struct ezi_software_table *tbl, const char *name);

/*!
 * \brief Loads a software table pointed to by \see tbl from its data source
 *
 * \param [in,out]  tbl  A ponter to the table which to load
 *
 * \return 0 on success, -1 on success with errno set
 */
int
ezi_software_table_load(struct ezi_software_table *tbl);

/*!
 * \brief Saves a software table pointed to by \see tbl into its data source
 *
 * \param [in,out]  tbl  A ponter to the table which to save
 *
 * \return 0 on success, -1 on success with errno set
 */
int
ezi_software_table_save(struct ezi_software_table *tbl);

/*!
 * \brief Frees used resources by a software table pointed to by \see tbl
 *
 * \param [in,out]  tbl  A pointer to the software table object to be freed
 */
void
free_ezi_software_table(struct ezi_software_table *tbl);

/*!
 * \brief Frees used resources by a software database pointed to by \see db
 *
 * \param [in,out]  db  A pointer to the software database object to be freed
 */
void
free_ezi_software_database(struct ezi_software_database *db);

#endif

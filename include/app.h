#ifndef EZI_APP_H
#define EZI_APP_H

/*!
 * \brief Runs the application
 *
 * \param [in] argc  The command line arguments count
 * \param [in] argv  The command line arguments vector
 * \return 0 for success, -1 for errors with errno set
 */
int
ezi_app_run(int argc, char *argv[]);

/*!
 * \brief Frees all used resources used by the application
 */
void
ezi_app_destroy(void);

#endif

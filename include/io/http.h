#ifndef EZI_IO_HTTP_H
#define EZI_IO_HTTP_H

#include <time.h>

struct ezi_http_context
{
    const char *url;
    const char *filename;
    time_t start_time;
};

typedef int (*http_progress_func)(struct ezi_http_context *ctx,
                                  double                         tdownload,
                                  double                         ndownload,
                                  double                         tupload,
                                  double                         cupload);

int
ezi_http_download(const char *       url,
                  const char *       filename,
                  http_progress_func progress_cb);

int
ezi_http_download_progress(struct ezi_http_context *ctx,
                           double                         tdownload,
                           double                         ndownload,
                           double                         tupload,
                           double                         cupload);

#endif

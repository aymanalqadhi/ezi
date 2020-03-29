#include "io/http.h"

#include "log/errno.h"
#include "log/logger.h"
#include "util/validation.h"

#include <curl/curl.h>

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

static void
encode_mm_ss_time(time_t time, char *buf, size_t len);

static void
bytes_to_readable_fmt(double bytes, char *buf, size_t len);

int
ezi_http_download(const char *       url,
                  const char *       filename,
                  http_progress_func progress_cb)
{
    FILE *                  fp;
    CURL *                  curl;
    CURLcode                res;
    struct ezi_http_context ctx;

    CHECK_NULL_PARAMS_2(url, filename);

    if (!(curl = curl_easy_init())) {
        errno = EZI_ERR_MEMORY_ALLOC_FAILED;
        return -1;
    }

    if (!(fp = fopen(filename, "w"))) {
        errno = EZI_ERR_FILE_OPEN_FAILED;
        curl_easy_cleanup(curl);
        return -1;
    }

    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &fwrite);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);

    if (progress_cb) {
        ctx.url        = url;
        ctx.filename   = filename;
        ctx.start_time = time(NULL);

        curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0);
        curl_easy_setopt(curl, CURLOPT_PROGRESSDATA, (void *)&ctx);
        curl_easy_setopt(
            curl,
            CURLOPT_PROGRESSFUNCTION,
            (int (*)(void *, double, double, double, double))progress_cb);

        putchar('\n');
    }

    if ((res = curl_easy_perform(curl))) {
        log_error("http_download", "%s", curl_easy_strerror(res));

        errno = EZI_ERR_FILE_DOWNLOAD_FAILED;
        curl_easy_cleanup(curl);
        fclose(fp);

        return -1;
    }

    if (progress_cb) {
        putchar('\n');
    }

    curl_easy_cleanup(curl);
    fclose(fp);

    return 0;
}

int
ezi_http_download_progress(struct ezi_http_context *ctx,
                           double                   tdownload,
                           double                   ndownload,
                           double                   tupload,
                           double                   cupload)
{
    double tpbar_cells, pbar_cells, done;
    char   time_buf[8], total_buf[0x10], done_buf[0x10];

    if (tdownload <= 0) {
        return 0;
    }

    done        = ndownload / tdownload;
    tpbar_cells = 50;
    pbar_cells  = tpbar_cells * done;
    tpbar_cells -= pbar_cells;

    encode_mm_ss_time(time(NULL) - ctx->start_time, time_buf, sizeof(time_buf));

    bytes_to_readable_fmt(tdownload, total_buf, sizeof(total_buf));
    bytes_to_readable_fmt(ndownload, done_buf, sizeof(done_buf));

    printf("%-40s [%-6s / %6s] [%s] [",
           ctx->filename,
           done_buf,
           total_buf,
           time_buf);

    while (pbar_cells > 0) {
        putchar(pbar_cells < 1 ? '>' : '=');
        --pbar_cells;
    }

    while (tpbar_cells-- > 0) {
        putchar(' ');
    }

    printf("] %3.2f%%     \r", done * 100);
    fflush(stdout);

    return 0;
}

static void
encode_mm_ss_time(time_t time, char *buf, size_t len)
{
    snprintf(buf, len, "%02lu:%02lu", time / 60UL, time % 60UL);
}

static void
bytes_to_readable_fmt(double bytes, char *buf, size_t len)
{
    switch ((int)floor(log2l(bytes)) / 10) {
    case 1:
        snprintf(buf, len, "%3.2f KiB", bytes / 1024);
        return;
    case 2:
        snprintf(buf, len, "%3.2f MiB", bytes / 1024 / 1024);
        return;
    case 3:
        snprintf(buf, len, "%3.2f GiB", bytes / 1024 / 1024 / 1024);
        return;
    case 4:
        snprintf(buf, len, "%3.2f TiB", bytes / 1024 / 1024 / 1024 / 1024);
        return;
    default:
        snprintf(buf, len, "%3f.0 B", bytes);
        return;
    }
}

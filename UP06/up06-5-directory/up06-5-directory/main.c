//
//  main.c
//  up06-5-directory
//
//  Created by Мамаев Павел on 21/10/2018.
//  Copyright © 2018 Мамаев Павел. All rights reserved.
//

#include <dirent.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <limits.h>
int compare(const void * x1, const void * x2) {
    return (strcasecmp(*(char**)x1, *(char**)x2));
}
int traverse(const char *dir)
{
    DIR *d = opendir(dir);
    if (d == NULL) {
        return 1;
    }
    char path[PATH_MAX];
    struct dirent *dd;
    int count = 0;
    int all = 2;
    char **v = calloc(all, sizeof(*v));
    if (v == NULL){
        return 1;
    }
    while ((dd = readdir(d))) {
        if (!strcmp(dd->d_name, ".") || !strcmp(dd->d_name, ".."))
            continue;
        int slen = snprintf(path, sizeof(path), "%s/%s", dir, dd->d_name);
        if (slen + 1 > sizeof(path)) {
            continue;
        }
        struct stat info;
        if (lstat(path, &info) < 0) continue;
        if (S_ISDIR(info.st_mode)) {
            if (count == all) {
                v = realloc(v, (all *= 2) * sizeof(*v));
            }
            v[count++] = strdup(dd->d_name);
        }
    }
    closedir(d);
    qsort(v, count, sizeof(*v), compare);
    for (size_t i = 0; i < count; ++i) {
        snprintf(path, sizeof(path), "%s/%s", dir, v[i]);
        printf("cd %s\n",v[i]);
        traverse(path);
        printf("cd ..\n");
        free(v[i]);
    }
    free(v);
    return 0;
}

int main(int argc, char *argv[])
{
    traverse(argv[1]);
    return 0;
}

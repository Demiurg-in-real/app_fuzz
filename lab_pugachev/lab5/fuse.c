#define FUSE_USE_VERSION 26

#include <fuse.h>
#include <string.h>
#include <errno.h>

static const char *filepath = "/file";
static const char *filename = "file";
static const char *filecontent = "I'm the content of the only file available there\n";

static int getattr_callback(const char *path, struct stat *stbuf) {
  memset(stbuf, 0, sizeof(struct stat));

  if (strcmp(path, "/") == 0) {
    stbuf->st_mode = S_IFDIR | 0755; //сфоткал, 0-битовое представление, первая часть c правами доступа для (на фото)	
    stbuf->st_nlink = 2; //1 link - "." , 2 link - "/" 
    return 0;
  }

  if (strcmp(path, filepath) == 0) { //тут сравнивается path и filepath и если она выдаёт false(==0), то работает...
    stbuf->st_mode = S_IFREG | 0777;//почему для папки и для файла разные? хотя...
    stbuf->st_nlink = 1; 
    stbuf->st_size = strlen(filecontent);
    return 0;
  }

  return -ENOENT;
}

static int readdir_callback(const char *path, void *buf, fuse_fill_dir_t filler,
    off_t offset, struct fuse_file_info *fi) {//fuse_fill_dir_it - почитать библиотеку для fuse
  (void) offset;
  (void) fi;//шо це такэ?

  filler(buf, ".", NULL, 0);
  filler(buf, "..", NULL, 0);//каким образом тут определяется порядок? не догоняю

  filler(buf, filename, NULL, 0);

  return 0;
}

static int open_callback(const char *path, struct fuse_file_info *fi) {//права на открытие?
  return 0;
}

static int read_callback(const char *path, char *buf, size_t size, off_t offset,
    struct fuse_file_info *fi) {//порядок чтения

  if (strcmp(path, filepath) == 0) { // и всё же, не догоняю - зачем это сравнение? Но можно проигнорить этот вопрос
    size_t len = strlen(filecontent);
    if (offset >= len) {
      return 0;
    }

    if (offset + size > len) {
      memcpy(buf, filecontent + offset, len - offset);
      return len - offset;
    }

    memcpy(buf, filecontent + offset, size);
    return size;
  }

  return -ENOENT;
}

static struct fuse_operations fuse_example_operations = {
  .getattr = getattr_callback,      //почему через точку?
  .open = open_callback,
  .read = read_callback,//qqQqqqQqqqq
  .readdir = readdir_callback,
};

int main(int argc, char *argv[])
{
  return fuse_main(argc, argv, &fuse_example_operations, NULL); //fuse_main - тупой вопрос, но где она? без неё файл собрался и заработал... на том примере у кати
}

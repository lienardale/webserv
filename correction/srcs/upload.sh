rm ../www/uploads/*
echo "Files in /www/uploads before upload :"
echo "--> "
ls ../www/uploads
echo

curl localhost:8000/upload.php -X POST -H "content-type: multipart/form-data; boundary=--------------------------045354470775848538579657" --data "----------------------------045354470775848538579657
Content-Disposition: form-data; name=\"file\"; filename=\"upload_test_1.c\"
Content-Type: text/x-c

#include <magic.h>

#include <magic.h>
#include <stdio.h>

int main(void) {
  char actual_file[] = \"test.cpp\";
  const char *magic_full;
  magic_t magic_cookie;

  /* MAGIC_MIME tells magic to return a mime of the file,
     but you can specify different things       */
  magic_cookie = magic_open(MAGIC_MIME);

  if (magic_cookie == NULL) {
    printf(\"unable to initialize magic library\n\");
    return 1;
  }

  printf(\"Loading default magic database\n\");

  if (magic_load(magic_cookie, NULL) != 0) {
    printf(\"cannot load magic database - %s\n\", magic_error(magic_cookie));
    magic_close(magic_cookie);
    return 1;
  }

  magic_full = magic_file(magic_cookie, actual_file);
  printf("%s\n", magic_full);
  magic_close(magic_cookie);
  return 0;
}
----------------------------045354470775848538579657
Content-Disposition: form-data; name=\"file2\"; filename=\"upload_test_2.txt\"
Content-Type: text/x-c

#include <magic.h>

#include <magic.h>
#include <stdio.h>

int main(void) {
  char actual_file[] = \"test.cpp\";
  const char *magic_full;
  magic_t magic_cookie;

  /* MAGIC_MIME tells magic to return a mime of the file,
     but you can specify different things       */
  magic_cookie = magic_open(MAGIC_MIME);

  if (magic_cookie == NULL) {
    printf(\"unable to initialize magic library\n\");
    return 1;
  }

  printf(\"Loading default magic database\n\");

  if (magic_load(magic_cookie, NULL) != 0) {
    printf(\"cannot load magic database - %s\n\", magic_error(magic_cookie));
    magic_close(magic_cookie);
    return 1;
  }

  magic_full = magic_file(magic_cookie, actual_file);
  printf(\"%s\n\", magic_full);
  magic_close(magic_cookie);
  return 0;
}
----------------------------045354470775848538579657--"

echo
echo "Files in /www/uploads after upload :"
echo -n "--> "
ls ../www/uploads
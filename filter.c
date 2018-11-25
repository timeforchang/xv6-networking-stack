#include "types.h"
#include "user.h"

int mac_conforms(char* src) {
	int count = 0;
	while (src[count] != '\0') {
		count++;
	}

	if (count != 17) {
		return -1;
	}

	for (int i = 0; i < count; i++) {
		if (i == 2 || i == 5 || i == 8 || i == 11 || i == 14) {
			if (src[i] != ':') {
				return -1;
			} else {
				//printf(1, "colon OK\n");
			}
		} else {
			if (src[i] >= 'A' && src[i] <= 'F') {
				//printf(1, "char OK\n");
			} else {
				return -1;
			}
		}
	}

	return 0;
}

int dir_conforms(char* src) {
	int count = 0;
	while (src[count] != '\0') {
		count++;
	}

	if (count < 2 || count > 3) {
		return -1;
	}

	char* in = (char*)"IN";
	char* out = (char*)"OUT";

	printf(1, "src: %s\n", src);

	if (strcmp(src, in) == 0 || strcmp(src, out) == 0) {
		printf(1, "dir OK\n");
	} else {
		return -1;
	}

	return 0;
}

int opt_conforms(char* src) {
	int count = 0;
	while (src[count] != '\0') {
		count++;
	}

	if (count != 2) {
		return -1;
	}

	char* add = (char*)"-A";
	char* del = (char*)"-D";

	printf(1, "src: %s\n", src);

	if (strcmp(src, add) == 0) {
		return 1;
	} else if (strcmp(src, del) == 0) {
		return 0;
	} else {
		return -1;
	}
}

int main(int argc, char *argv[]) {
	if (argc != 4) {
		printf(1, "Unable to set new filter rule.\nUSAGE: filter <OPT> <MAC> <IN/OUT>.\n");
		exit();
	}

	if (mac_conforms(argv[2]) < 0) {
		printf(1, "MAC address doesn't conform, please make sure all letters are in caps and it is in the correct MAC address format\n");
		exit();
	}

	if (dir_conforms(argv[3]) < 0) {
		printf(1, "direction doesn't conform, please type 'IN' or 'OUT'\n");
		exit();
	}

	int opt = opt_conforms(argv[1]);

	if (opt < 0) {
		printf(1, "not a valid option, please use -A to add or -D to delete rule\n");
		exit();
	}

    if (filter("mynet0", opt, argv[2], argv[3])) {
        printf(1, "Set new rule failed.\n");
    }

  exit();
}

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>


#pragma pack(push, 1)
struct User {
    char username[32];
    char password[31];
    uint32_t value;
};
#pragma pack(pop)

int save_user(const char* path, struct User const* user) {
    FILE* f;
    return (
        (f = fopen(path, "w")) != NULL
        && fwrite(user, sizeof(*user), 1, f) == 1
        && fclose(f) != -1
    ) ? 0 : -1;
}

int load_user(const char* path, struct User* user) {
    FILE* f;
    return (
        (f = fopen(path, "r")) != NULL
        && fread(user, sizeof(*user), 1, f) == 1
        && fclose(f) != -1
    ) ? 0 : -1;
}

int print_user(struct User* user) {
    printf("name: %.32s, password: %.31s value: %" PRIu32 "\n", user->username, user->password, user->value);
    return 0;
}

int main() {
    struct User user = {
        .username = "abcdnckhenfkcenkfcfckcenkfcfcjfd",
        .password = "khsdnckhenfkcenkfcfckcenkfcfcjd",
        .value = 0x31313131,
    };
    struct User user_loaded;
    int ok = (
        save_user("data.bin", &user) != -1
        && load_user("data.bin", &user_loaded) != -1
        && print_user(&user_loaded) != -1
    );
    if (!ok) {
        perror("Some error");
        return -1;
    }
    return 0;
}
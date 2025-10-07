#ifndef AUTH_H
#define AUTH_H

#include <bricli/bricli.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

    extern bool Bricli_IsCommandInScope(BricliHandle_t *cli, BricliCommand_t *command);

#ifdef __cplusplus
}
#endif // __cplusplus
#endif // AUTH_H

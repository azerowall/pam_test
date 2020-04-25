#include <iostream>

using std::cin;
using std::cout;
using std::endl;
using std::string;

#include <security/pam_appl.h>
#include <security/pam_misc.h>

static const char * SERVICE_NAME = "test_pam_app";

int conv_cb(int num_msg, const struct pam_message **msg, struct pam_response **resp, void *appdata_ptr)
{
    string input;

    *resp = (struct pam_response *) calloc(num_msg, sizeof(struct pam_response));

    for (int i = 0; i < num_msg; i++) {
        const struct pam_message *m = msg[i];
        resp[i]->resp_retcode = 0;

        switch (m->msg_style)
        {
        case PAM_PROMPT_ECHO_ON:
            cout << "[echo on] " << m->msg;
            cin >> input;
            resp[i]->resp = strdup(input.c_str());
            break;

        case PAM_PROMPT_ECHO_OFF:
            resp[i]->resp = getpass(m->msg);
            break;

        case PAM_ERROR_MSG:
            cout << "[error] " << m->msg << endl;
            break;
        
        case PAM_TEXT_INFO:
            cout << "[info] " << m->msg << endl;
            break;

        default:
            break;
        }
    }
    return PAM_SUCCESS;
}


int main(int argc, char **argv)
{
    string username;
    pam_handle_t *pamh;
    pam_conv pamconv;
    int retval;

    cout << "Username: ";
    cin >> username;

    pamconv.conv = conv_cb;
    pamconv.appdata_ptr = NULL;

    retval = pam_start(SERVICE_NAME, username.c_str(), &pamconv, &pamh);
    if (retval == PAM_SUCCESS) {
        retval = pam_authenticate(pamh, 0);
        if (retval == PAM_SUCCESS) {
            cout << "Success!!!" << endl;
        } else {
            cout << "authentication failed (status " << retval << ")" << endl;
        }
    } else {
        cout << "pam_start failed " << retval << endl;
    }

    pam_end(pamh, retval);

    return 0;
}
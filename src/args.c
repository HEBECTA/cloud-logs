#include "args.h"
#include <string.h>

const char *argp_program_version =
  "logs";
const char *argp_program_bug_address =
  "<log@logs.org>";

char doc[] =
  "a program with options to print logs";

char args_doc[] = "ARG1 ARG2";

struct argp_option options[] = {
  {"orgID",   'o', "name", 0,
   "organisation ID string" },
  {"typeId",   'p', "name", 0,
   "type ID string" },
  {"deviceId",   'd', "name", 0,
   "device ID string" },
  {"token",   't', "token", 0,
   "auth token string" },
   {0}
};

struct argp argp = { options, parse_opt, NULL, doc };


error_t
parse_opt (int key, char *arg, struct argp_state *state)
{

  struct arguments *arguments = state->input;

  switch (key)
    {
        case 'o':
                arguments->orgID_str = arg;
        break;
        case 'p':
                arguments->typeID_str = arg;
        break;
        case 'd':
                arguments->deviceID_str = arg;
        break;
        case 't':
                arguments->token_str = arg;
        break;

        default:
                return ARGP_ERR_UNKNOWN;
    }

  return 0;
}

int args_parse(int argc, char *argv[], struct arguments *args){

        int rc  = argp_parse (&argp, argc, argv, ARGP_NO_EXIT, 0, args);

        const char *empty = "-"; 

        if ( !strcmp(args->orgID_str, empty) || !strcmp(args->orgID_str, empty) ||
        !strcmp(args->orgID_str, empty) || !strcmp(args->orgID_str, empty) )
                rc = EINVAL;

        return rc;
}
#ifndef CONFIG_H
#define CONFIG_H


#define STB_SELF_ID                     STB_SERVER_ID //change it to own id

#define STB_NEXTCHAR_INTERVAL           100        ///<    MiliSec
#define STB_TIMEOUT                     1000


#define STB_SERVER_ID                   'C'
#define STB_ALIAS_SERVER                'P'
#define STB_ALIAS_TOKENBOOTH            'T'
#define STB_ALIAS_SERVICEBOOTH          'S'

#define STB_SERVER_PING_SERVICE         'p' /// ping the service booth
#define STB_SERVER_NEW_USER             'n' /// send a user to service booth
#define STB_SERVER_PING_TOKEN           'p' /// ping the token booth
#define STB_SERVER_DONE_USER            'D' /// say token booth that it's done with the new user

#define STB_SERVICE_FREE                'F' /// service booth is free
#define STB_SERVICE_BUSY                'B' /// service booth is busy
#define STB_SERVICE_TAKEN_USER          't' /// service booth has taken the user

#define STB_TOKEN_NO_USER               'n' /// token booth has no new user
#define STB_TOKEN_HAS_USER              'u' /// token booth has new user
#define STB_TOKEN_DONE_USER             'd' /// token booth has processed the user


#define STB_CATEGORY_WITHDRAW           'w'
#define STB_CATEGORY_DEPOSIT            'd'
#define STB_CATEGORY_HELP               'h'


#define NUM_SERVICE_BOOTH               2
#define NUM_TOKEN_BOOTH                 1 /// currently can handle only one token booth

#endif // CONFIG_H

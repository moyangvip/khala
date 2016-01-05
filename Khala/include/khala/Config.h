/*
 * Config.h
 *
 *  Created on: Nov 25, 2015
 *      Author: moss
 */

#ifndef CONFIG_H_
#define CONFIG_H_
namespace khala {
#define USER_PORT 2007

#define USER_SERVER_THREAD_NUM 4
/*
 * if DEFAULT_IDLE_ALIVE_TIME = 0,never overtime
 * */
#define DEFAULT_IDLE_ALIVE_TIME 0
#define DEFAULT_CHECK_IDLE_TIME 10

/*
 * DB
 * #define MAX_DB_CONN 4
#define DB_NAME "HomeServerDB"
#define DB_ADDR "127.0.0.1"
#define DB_USER_NAME "root"
#define DB_PW "******"
 * */

}

#endif /* CONFIG_H_ */

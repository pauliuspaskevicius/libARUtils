/**
 * @file ARUTILS_Ftp.h
 * @brief libARUtils Ftp header file.
 * @date 19/12/2013
 * @author david.flattin.ext@parrot.com
 **/

#ifndef _ARUTILS_FTP_PRIVATE_H_
#define _ARUTILS_FTP_PRIVATE_H_

#include "libARUtils/ARUTILS_Manager.h"

/**
 * @brief Ftp command Delete file
 */
#define FTP_CMD_DELE    "DELE "

/**
 * @brief Ftp command List directory
 */
#define FTP_CMD_LIST    "LIST "

/**
 * @brief Ftp command Name List directory
 */
#define FTP_CMD_NLST    "NLST "

/**
 * @brief Ftp command Rename From origine name
 */
#define FTP_CMD_RNFR    "RNFR "

/**
 * @brief Ftp command Rename To destination name
 */
#define FTP_CMD_RNTO    "RNTO "

/**
 * @brief Ftp command Remove Directory
 */
#define FTP_CMD_RMD     "RMD "

/**
 * @brief Ftp command Make Directory
 */
#define FTP_CMD_MKD     "MKD "

/**
 * @brief Ftp command Size get file size
 */
#define FTP_CMD_SIZE    "SIZE "

/**
 * @brief Ftp command Change working directory
 */
#define FTP_CMD_CWD     "CWD "

/**
 * @brief Ftp max user name string size
 */
#define ARUTILS_FTP_MAX_USER_SIZE     64

/**
 * @brief Ftp CallbackData structure
 * @param isUploading Is set to 1 if upploading else 0 of downloading
 * @param data The byte buffer data if data mode else null
 * @param dataSize The byte buffer data size if data mode else 0
 * @param file The file data if file mode else null
 * @param error The last error
 * @param progressCallback The progress callback
 * @param progressArg The progress arg
 * @see ARUTILS_Ftp_ReadDataCallback (), ARUTILS_Ftp_WriteDataCallback ()
 */
typedef struct _ARUTILS_Ftp_CallbackData_t_
{
    int isUploading;
    uint8_t *data;
    uint32_t dataSize;
    FILE *file;
    eARUTILS_ERROR error;
    ARUTILS_Ftp_ProgressCallback_t progressCallback;
    void *progressArg;
    
} ARUTILS_Ftp_CallbackData_t;

/**
 * @brief Ftp Connection structure
 * @param cancelSem The semaphore to cancel Ftp command
 * @param curl The cURL connection
 * @param serverUrl The Ftp url connection string
 * @param username The Ftp connection user name
 * @param passwordThe Ftp connection user password
 * @param cbdata The Ftp connection data for callbacks
 * @see ARUTILS_Ftp_ReadDataCallback (), ARUTILS_Ftp_WriteDataCallback ()
 */
struct ARUTILS_Ftp_Connection_t
{
    ARSAL_Sem_t *cancelSem;
    CURL *curl;
    char serverUrl[ARUTILS_FTP_MAX_URL_SIZE];
    char username[ARUTILS_FTP_MAX_USER_SIZE];
    char password[ARUTILS_FTP_MAX_USER_SIZE];
    ARUTILS_Ftp_CallbackData_t cbdata;
};

/**
 * @brief ReadData callback of cURL connection
 * @param ptr The pointer of read data
 * @param size The size of the read data type (byte)
 * @param nmemb The number of read data present
 * @param userData The pointer of the user custom argument
 * @retval On success, returns nmemb. Otherwise, it returns an error code.
 * @see cURL
 */
size_t ARUTILS_Ftp_ReadDataCallback(void *ptr, size_t size, size_t nmemb, void *userData);

/**
 * @brief WriteData callback of cURL connection
 * @param ptr The pointer of write data
 * @param size The size of the write data type (byte)
 * @param nmemb The number of write data present
 * @param userData The pointer of the user custom argument
 * @retval On success, returns nmemb. Otherwise, it returns an error code.
 * @see cURL
 */
size_t ARUTILS_Ftp_WriteDataCallback(void *ptr, size_t size, size_t nmemb, void *userData);

/**
 * @brief Progress callback of cURL connection
 * @param userData The pointer of the user custom argument
 * @param dltotal The total size to be downloaded
 * @param dlnow The current size already donloaded
 * @param ultotal The total size to be uploaded
 * @param ulnow The current size already uploaded
 * @retval On success, returns 0. Otherwise, it returns an error code.
 * @see cURL
 */
int ARUTILS_Ftp_ProgressCallback(void *userData, double dltotal, double dlnow, double ultotal, double ulnow);

/**
 * @brief Execute a command on remote Ftp server file
 * @param connection The address of the pointer on the Ftp Connection
 * @param namePath The string of the file name path
 * @param command The string of the Ftp command
 * @param[out] ftpCode The resule code of the Ftp command
 * @retval On success, returns ARUTILS_OK. Otherwise, it returns an error number of eARUTILS_ERROR.
 * @see cURL
 */
eARUTILS_ERROR ARUTILS_Ftp_Command(ARUTILS_Ftp_Connection_t *connection, const char *namePath, const char *command, long *ftpCode);

/**
 * @brief Reset the Ftp connection values
 * @param connection The address of the pointer on the Ftp Connection
 * @retval On success, returns ARUTILS_OK. Otherwise, it returns an error number of eARUTILS_ERROR.
 * @see cURL
 */
eARUTILS_ERROR ARUTILS_Ftp_ResetOptions(ARUTILS_Ftp_Connection_t *connection);

/**
 * @brief Execute Cd command on remote Ftp server file
 * @param connection The address of the pointer on the Ftp Connection
 * @param namePath The string of the file name path
 * @retval On success, returns ARUTILS_OK. Otherwise, it returns an error number of eARUTILS_ERROR.
 * @see cURL
 */
eARUTILS_ERROR ARUTILS_Ftp_Cd(ARUTILS_Ftp_Connection_t *connection, const char *namePath);

/**
 * @brief Execute Get command on remote Ftp server file
 * @param connection The address of the pointer on the Ftp Connection
 * @param namePath The string of the file name path
 * @param dstFile The destination string file name path
 * @param[out] data Returns byte buffer data address if data mode else give null pointer
 * @param[out] dataLen Returns byte buffer data length else give null pointer
 * @param progressCallback The progress callback
 * @param progressArg The progress callback arg
 * @param resume The resune enun requested action
 * @retval On success, returns ARUTILS_OK. Otherwise, it returns an error number of eARUTILS_ERROR.
 * @see cURL
 */
eARUTILS_ERROR ARUTILS_Ftp_GetInternal(ARUTILS_Ftp_Connection_t *connection, const char *namePath, const char *dstFile, uint8_t **data, uint32_t *dataLen, ARUTILS_Ftp_ProgressCallback_t progressCallback, void* progressArg, eARUTILS_FTP_RESUME resume);

/**
 * @brief Free CallbackData structure
 * @warning This function frees memory
 * @param connection The address of the pointer on the Ftp Connection
 * @param namePath The string of the file name path
 * @see cURL
 */
void ARUTILS_Ftp_FreeCallbackData(ARUTILS_Ftp_CallbackData_t *cbdata);

/**
 * @brief Translate cURL error code to an eARUTILS_ERROR enum error
 * @param connection The address of the pointer on the Ftp Connection
 * @param code The cURL error code
 * @retval On success, returns ARUTILS_OK. Otherwise, it returns an error number of eARUTILS_ERROR.
 * @see cURL
 */
eARUTILS_ERROR ARUTILS_Ftp_GetErrorFromCode(ARUTILS_Ftp_Connection_t *connection, CURLcode code);


/**
* @brief Cancel an Ftp Connection command in progress (get, put, list etc)
* @param connection The address of the pointer on the Ftp Connection
* @retval On success, returns ARUTILS_OK. Otherwise, it returns an error number of eARUTILS_ERROR.
* @see ARUTILS_Manager_NewWifiFtp ()
*/
eARUTILS_ERROR ARUTILS_WifiFtpAL_Connection_Cancel(ARUTILS_Manager_t *manager);

/**
 * @brief Execute Ftp List command to retrieve directory content
 * @warning This function allocates memory
 * @param manager The address of the pointer on the Ftp Connection
 * @param namePath The string of the directory path on the remote Ftp server
 * @param resultList The pointer of the string of the directory content null terminated
 * @param resultListLen The pointer of the lenght of the resultList string including null terminated
 * @retval On success, returns ARUTILS_OK. Otherwise, it returns an error number of eARUTILS_ERROR.
 * @see ARUTILS_Manager_NewWifiFtp ()
 */
eARUTILS_ERROR ARUTILS_WifiFtpAL_List(ARUTILS_Manager_t *manager, const char *namePath, char **resultList, uint32_t *resultListLen);

/**
 * @brief Get an remote Ftp server file
 * @warning This function allocates memory
 * @param manager The address of the pointer on the Ftp Connection
 * @param namePath The string of the file name path on the remote Ftp server
 * @param data The pointer of the data buffer of the file data
 * @param dataLen The pointer of the length of the data buffer
 * @param progressCallback The progress callback function
 * @param progressArg The progress callback function arg
 * @retval On success, returns ARUTILS_OK. Otherwise, it returns an error number of eARUTILS_ERROR.
 * @see ARUTILS_Manager_NewWifiFtp (), ARUTILS_Ftp_ProgressCallback_t, eARUTILS_FTP_RESUME
 */
eARUTILS_ERROR ARUTILS_WifiFtpAL_Get_WithBuffer(ARUTILS_Manager_t *manager, const char *namePath, uint8_t **data, uint32_t *dataLen,  ARUTILS_Ftp_ProgressCallback_t progressCallback, void* progressArg);

/**
 * @brief Get an remote Ftp server file
 * @param manager The address of the pointer on the Ftp Connection
 * @param namePath The string of the file name path on the remote Ftp server
 * @param dstFile The string of the local file name path to be get
 * @param progressCallback The progress callback function
 * @param progressArg The progress callback function arg
 * @param resume The resume capability requested
 * @retval On success, returns ARUTILS_OK. Otherwise, it returns an error number of eARUTILS_ERROR.
 * @see ARUTILS_Manager_NewWifiFtp (), ARUTILS_Ftp_ProgressCallback_t, eARUTILS_FTP_RESUME
 */
eARUTILS_ERROR ARUTILS_WifiFtpAL_Get(ARUTILS_Manager_t *manager, const char *namePath, const char *dstFile, ARUTILS_Ftp_ProgressCallback_t progressCallback, void* progressArg, eARUTILS_FTP_RESUME resume);

/**
 * @brief Put an remote Ftp server file
 * @param manager The address of the pointer on the Ftp Connection
 * @param namePath The string of the file name path on the remote Ftp server
 * @param srcFile The string of the local file name path to be put
 * @param progressCallback The progress callback function
 * @param progressArg The progress callback function arg
 * @param resume The resume capability requested
 * @retval On success, returns ARUTILS_OK. Otherwise, it returns an error number of eARUTILS_ERROR.
 * @see ARUTILS_Manager_NewWifiFtp (), ARUTILS_Ftp_ProgressCallback_t, eARUTILS_FTP_RESUME
 */
eARUTILS_ERROR ARUTILS_WifiFtpAL_Put(ARUTILS_Manager_t *manager, const char *namePath, const char *srcFile, ARUTILS_Ftp_ProgressCallback_t progressCallback, void* progressArg, eARUTILS_FTP_RESUME resume);

/**
 * @brief Delete an remote Ftp server file
 * @param delete The address of the pointer on the Ftp Connection
 * @param namePath The string of the file name path on the remote Ftp server
 * @retval On success, returns ARUTILS_OK. Otherwise, it returns an error number of eARUTILS_ERROR.
 * @see ARUTILS_Manager_NewWifiFtp ()
 */
eARUTILS_ERROR ARUTILS_WifiFtpAL_Delete(ARUTILS_Manager_t *manager, const char *namePath);

#endif /* _ARUTILS_FTP_PRIVATE_H_ */

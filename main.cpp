/**
 * @ «‡ø’§¿§±§¨“ä§ø§§§Œ§œŒ“Éç§«§π§´
 * @Author       : RagnaLP
 * @Date         : 2023-05-23 15:17:14
 * @LastEditTime : 2023-05-23 16:23:10
 * @Description  :
 */

#include "file_system.h"

FileSystem file_system;
int main() {
    file_system.SignUp("root", "admin");
    file_system.Login("root", "123");
    system("pause");
    return 0;
}
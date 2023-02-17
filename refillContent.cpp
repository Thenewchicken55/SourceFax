/*
    refillContent.cpp

    Implementation file for the refillContent function
*/

#include "refillContent.hpp"
#include <errno.h>
#include <sys/types.h>

#if !defined(_MSC_VER)
#include <sys/uio.h>
#include <unistd.h>
#define READ read
#else
#include <BaseTsd.h>
#include <io.h>
typedef SSIZE_T ssize_t;
#define READ _read
#endif

const int BLOCK_SIZE = 4096;
const int BUFFER_SIZE = 16 * 16 * BLOCK_SIZE;

/*
    Refill the content preserving the existing data.

    @param[in, out] content View of the content
    @return Number of bytes read
    @retval 0 EOF
    @retval -1 Read error
*/
[[nodiscard]] int refillContent(std::string_view& content) {

    // initialize the internal buffer at first use
    static char buffer[BUFFER_SIZE];

    // preserve prefix of unprocessed characters to start of the buffer
    std::copy(content.cbegin(), content.cend(), buffer);

    // read in multiple of whole blocks
    ssize_t bytesRead = 0;
    while (((bytesRead = READ(0, (buffer + content.size()),
        BUFFER_SIZE - BLOCK_SIZE)) == -1) && (errno == EINTR)) {
    }
    if (bytesRead == -1) {
        // error in read
        return -1;
    }

    // set content to the start of the buffer
    content = std::string_view(buffer, content.size() + bytesRead);

    return bytesRead;
}

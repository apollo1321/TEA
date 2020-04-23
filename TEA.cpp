#include "TEA.h"
#include <iomanip>


std::array<unsigned int, 2> decipher (std::array<unsigned int, 2> input,
                                      const std::array<unsigned int, 4>&  key)
{
    static_assert (sizeof (int) == 4, "Wrong int size");

    unsigned int sum = 0xC6EF3720;
    const unsigned int delta = 0x9E3779B9;

    for (unsigned int n = 32; n-- > 0; )
    {
        input[1] -= (input[0] << 4 ^ input[0] >> 5) + input[0] ^ sum + key[sum>>11 & 3];
        sum -= delta;
        input[0] -= (input[1] << 4 ^ input[1] >> 5) + input[1] ^ sum + key[sum&3];
    }

    return input;
}

std::array<unsigned int, 2> encipher (std::array<unsigned int, 2> input,
                                      const std::array<unsigned int, 4>&  key)
{
    static_assert (sizeof (int) == 4, "Wrong long size");

    unsigned int sum = 0;
    const unsigned int delta = 0x9E3779B9;

    for (unsigned int n = 32; n-- > 0; )
    {
        input[0] += (input[1] << 4 ^ input[1] >> 5) + input[1] ^ sum + key[sum&3];
        sum += delta;
        input[1] += (input[0] << 4 ^ input[0] >> 5) + input[0] ^ sum + key[sum>>11 & 3];
    }

    return input;
}

void encipher (std::istream& is, std::ostream& os, std::string key)
{
    constexpr int nchar = 8; //the size of input data in TEA algorithm
    constexpr int kchar = 2*nchar; //the size of key in TEA algorithm

    while (key.size() < kchar) key += '0';

    std::array<unsigned int, 4> key_arr;
    {
        const unsigned int* const p_key = reinterpret_cast<const unsigned int*> (key.data());
        std::copy (p_key, p_key + key_arr.size(), key_arr.begin());
    }

    char inbuf[nchar];
    const auto p_data = reinterpret_cast<const unsigned int*> (inbuf);

    int count = 0;
    os << std::hex;
    while (is.get (inbuf[count]))
    {
        if (++count == nchar)
        {
            auto res = encipher ({p_data[0], p_data[1]}, key_arr);
            os << std::setw(8) << std::setfill('0') << res[0] << ' '
               << std::setw(8) << std::setfill('0') << res[1] << ' ';
            count = 0;
        }
    }

    if (count)
    {
        while (count != nchar) inbuf[count++] = '\0';

        auto res = encipher ({p_data[0], p_data[1]}, key_arr);
        os << std::setw(8) << std::setfill('0') << res[0] << ' '
           << std::setw(8) << std::setfill('0') << res[1] << ' ';
    }
}

void decipher (std::istream& is, std::ostream& os, std::string key)
{
    constexpr int nchar = 8; //the size of input data in TEA algorithm
    constexpr int kchar = 2*nchar; //the size of key in TEA algorithm

    while (key.size() < kchar) key += '0';

    std::array<unsigned int, 4> key_arr;
    {
        const unsigned int* const p_key = reinterpret_cast<const unsigned int*> (key.data());
        std::copy (p_key, p_key + key_arr.size(), key_arr.begin());
    }

    char outbuf[nchar + 1];
    outbuf[nchar] = 0; // terminator

    is.setf (std::ios_base::hex, std::ios_base::basefield);

    for (std::array<unsigned int, 2> input; is >> input[0] >> input[1];)
    {
        auto res = decipher (input, key_arr);
        const auto p_res = reinterpret_cast<const char*>(res.data());
        std::string str_res;
        for (int i = 0; i < nchar; ++i) str_res.push_back(p_res[i]);
        os << str_res;
    }
}

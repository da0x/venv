/*

Copyright (C) 2024 Daher Alfawares

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <https://www.gnu.org/licenses/>.

*/

#ifndef UUID_HPP
#define UUID_HPP

#include <random>
#include <sstream>
#include <iomanip>
#include <string>
#include <cstdint>

namespace uuid {
    namespace v4 {
        // Generate a true version 4 UUID (random-based)
        static std::string generate() {
            std::random_device rd;
            std::mt19937_64 gen(rd());
            std::uniform_int_distribution<uint64_t> dis(0, (uint64_t)-1);

            uint64_t data1 = dis(gen);
            uint64_t data2 = dis(gen);

            // Set the version (4) and variant (RFC 4122)
            data1 = (data1 & 0xFFFFFFFFFFFF0FFFULL) | 0x0000000000004000ULL; // Version 4
            data2 = (data2 & 0x3FFFFFFFFFFFFFFFULL) | 0x8000000000000000ULL; // Variant 1

            std::ostringstream oss;
            oss << std::hex << std::setfill('0')
                << std::setw(8) << (data1 >> 32) << '-'
                << std::setw(4) << ((data1 >> 16) & 0xFFFF) << '-'
                << std::setw(4) << (data1 & 0xFFFF) << '-'
                << std::setw(4) << (data2 >> 48) << '-'
                << std::setw(12) << (data2 & 0xFFFFFFFFFFFFULL);

            return oss.str();
        }
    }

    // Function to convert a UUID string into a uint64_t by taking half of the UUID (lower 64 bits)
    static uint64_t uuid_to_uint64(const std::string& uuid_str) {
        // Extract the last 12 characters from the UUID string
        std::string last_part = uuid_str.substr(uuid_str.length() - 12);

        // Convert the last part of the UUID string to uint64_t
        return std::stoull(last_part, nullptr, 16);
    }
}

#endif // UUID_HPP

#pragma once

#include <Windows.h>

#include <stdint.h>
#include <vector>

class ConsoleEntity
{
public:
    ConsoleEntity();
    ConsoleEntity(const char* data, uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint16_t attribute);
    ConsoleEntity(const wchar_t* data, uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint16_t attribute);
    
    uint32_t GetX() const;
    uint32_t GetY() const;
    void SetData(const char* data, uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint16_t attribute);
    void SetData(char asciiChar, uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint16_t attribute);
    void SetData(const wchar_t* data, uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint16_t attribute);
    int WriteData(const char* data, uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint16_t attribute);
    int WriteData(uint32_t x, uint32_t y, uint16_t attribute, const char* format, ...);
    void SetPosition(uint32_t x, uint32_t y);
    void SetAttribute(uint16_t attribute);

    bool Draw(HANDLE consoleOutput) const;

private:
    uint32_t m_x = 0;
    uint32_t m_y = 0;
    uint32_t m_w = 0;
    uint32_t m_h = 0;

    HANDLE m_handle = nullptr;
    std::vector<CHAR_INFO> m_buffer;
};

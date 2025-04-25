#include "Entity.h"

ConsoleEntity::ConsoleEntity()
{
    m_handle = CreateConsoleScreenBuffer(
        GENERIC_READ | GENERIC_WRITE,
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        nullptr, // default security attributes
        CONSOLE_TEXTMODE_BUFFER, // must be TEXTMODE
        nullptr);
    if (m_handle == INVALID_HANDLE_VALUE)
        m_handle = nullptr;
}

ConsoleEntity::ConsoleEntity(const char* data, uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint16_t attribute)
{
	m_handle = CreateConsoleScreenBuffer(
		GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		nullptr, // default security attributes
		CONSOLE_TEXTMODE_BUFFER, // must be TEXTMODE
		nullptr);
	if (m_handle == INVALID_HANDLE_VALUE)
		m_handle = nullptr;

	SetData(data, x, y, w, h, attribute);
}

ConsoleEntity::ConsoleEntity(const wchar_t* data, uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint16_t attribute)
{
	m_handle = CreateConsoleScreenBuffer(
		GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		nullptr, // default security attributes
		CONSOLE_TEXTMODE_BUFFER, // must be TEXTMODE
		nullptr);
	if (m_handle == INVALID_HANDLE_VALUE)
		m_handle = nullptr;

	SetData(data, x, y, w, h, attribute);
}

uint32_t ConsoleEntity::GetX() const
{
    return m_x;
}

uint32_t ConsoleEntity::GetY() const
{
    return m_y;
}

void ConsoleEntity::SetData(const char* data, uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint16_t attribute)
{
    m_x = x;
    m_y = y;
    m_w = w;
    m_h = h;
    m_buffer.resize(m_w * m_h);
    memset(m_buffer.data(), 0, sizeof(CHAR_INFO) * (m_w * m_h));

    const int len = (int)strlen(data);
    for (uint32_t i = 0; i < m_w * m_h; i++)
    {
        if (i < (uint32_t)len)
            m_buffer[i].Char.AsciiChar = *(data + i);
        m_buffer[i].Attributes = attribute;
    }
}

void ConsoleEntity::SetData(const wchar_t* data, uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint16_t attribute)
{
	m_x = x;
	m_y = y;
	m_w = w;
	m_h = h;
	m_buffer.resize(m_w * m_h);
	memset(m_buffer.data(), 0, sizeof(CHAR_INFO) * (m_w * m_h));

	const int len = (int)wcslen(data);
	for (uint32_t i = 0; i < m_w * m_h; i++)
	{
		if (i < (uint32_t)len)
			m_buffer[i].Char.UnicodeChar = *(data + i);
		m_buffer[i].Attributes = attribute;
	}
}


void ConsoleEntity::SetData(char asciiChar, uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint16_t attribute)
{
    m_x = x;
    m_y = y;
    m_w = w;
    m_h = h;

    m_buffer.resize(m_w * m_h);
    memset(m_buffer.data(), 0, sizeof(CHAR_INFO) * (m_w * m_h));
    for (uint32_t i = 0; i < m_w * m_h; i++)
    {
        m_buffer[i].Char.AsciiChar = asciiChar;
        m_buffer[i].Attributes = attribute;
    }
}

void ConsoleEntity::SetPosition(uint32_t x, uint32_t y)
{
    m_x = x;
    m_y = y;
}

void ConsoleEntity::SetAttribute(uint16_t attribute)
{
    for (uint32_t i = 0; i < m_w * m_h; i++)
    {
        m_buffer[i].Attributes = attribute;
    }
}

int ConsoleEntity::WriteData(const char* data, uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint16_t attribute)
{
	const int len = (int)strlen(data);
	int dataPos = 0;
	
	int pos = (y * m_w) + x;
	for (int y_ = 0; y_ < (int)h; y_++)
	{
		for (int x_ = 0; x_ < (int)w; x_++)
		{
			int ignore = (int)'.';
			CHAR asciiCharacter = *(data + dataPos++);
			if ((int)asciiCharacter != ignore)
			{
				m_buffer[pos].Char.AsciiChar = asciiCharacter;
				m_buffer[pos].Attributes = attribute;
			}
			pos++;
		}
		pos = ((y + (y_ + 1)) * m_w) + x;
	}

	return len;
}

int ConsoleEntity::WriteData(uint32_t x, uint32_t y, uint16_t attribute, const char* format, ...)
{
	va_list args;
	va_start(args, format);

	char buffer[100] = { '\0' };
	vsnprintf(buffer, 100, format, args);
	WriteData(buffer, x, y, (int)strlen(buffer), 1, attribute);

	va_end(args);

	return (int)strlen(buffer);
}

bool ConsoleEntity::Draw(HANDLE consoleOutput) const
{
    SMALL_RECT coord{ (SHORT)m_x, (SHORT)m_y, (SHORT)(m_x + m_w), (SHORT)(m_y + m_h) }; // ltrb
    BOOL success = WriteConsoleOutputW(
        consoleOutput,
        m_buffer.data(),
        { (SHORT)m_w, (SHORT)m_h },
        { 0, 0 }, // Top left point in data
        &coord);
    return success == TRUE;
}
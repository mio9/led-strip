unsigned char nibble(char c)
{
    return c % 16 + 9 * (c >> 6);
}

unsigned int hex2num(const char *s)
{
    unsigned int r = 0;
    while (*s)
        r = (r << 4) | nibble(*s++);
    return r;
}
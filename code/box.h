#pragma once
#include <math.h>
#include <algorithm>

struct Box 
{
    float m_pos_x = 0.0f;
    float m_pos_y = 0.0f;
    float m_width = 1.0f;
    float m_height = 1.0f;
    int m_id = 0; //0 for platform blocks, 1 for good items , 2 for bad items, 3 for portal

    bool intersect(Box& other)
    {
        return (fabs(m_pos_x - other.m_pos_x) * 2.0f < (m_width + other.m_width)) &&
            (fabs(m_pos_y - other.m_pos_y) * 2.0f < (m_height + other.m_height));
    }


    float intersectDown(Box& other)
    {
        if (fabs(m_pos_x - other.m_pos_x) * 2.0f >= (m_width + other.m_width) || m_pos_y > other.m_pos_y)
            return 0.0f;
        return std::min<float>(0.0f, other.m_pos_y - (other.m_height / 2.0f) - m_pos_y - (m_height / 2.0f));
    }

    float intersectSideways(Box& other)
    {
        if (fabs(m_pos_y - other.m_pos_y) * 2.0f >= (m_width + other.m_width))
            return 0.0f;
        if (m_pos_x > other.m_pos_x)
            return std::max<float>(0.0f, other.m_pos_x + (other.m_width / 2.0f) - m_pos_x + (m_width / 2.0f));
        else
            return std::min<float>(0.0f, other.m_pos_x - (other.m_width / 2.0f) - m_pos_x - (m_width / 2.0f));
    }

   

    Box() {}

    Box(float x, float y, float w, float h, int id)
        : m_pos_x(x), m_pos_y(y), m_width(w), m_height(h), m_id(id) {}
    
    
    bool b_active = true;
public:
    bool bisActive() const { return b_active; } // Marked as const
    void bsetActive(bool a) { b_active = a; }
};

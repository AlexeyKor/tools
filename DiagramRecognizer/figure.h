#pragma once

class Figure : public Component
{
public:
    Figure(abstractRec...);  //���������� ������, ������ �� ���
    int getType() const;
    void setType(int value);

private:
    int type;
};

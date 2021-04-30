#ifndef APP_HPP
#define APP_HPP

class PM : public QWidget
{
public:
    PM(QWidget *parent, int w, int h);

private:
    int window_width;
    int window_height;
};

#endif // APP_HPP
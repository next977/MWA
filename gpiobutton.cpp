#include "gpiobutton.h"

GPIOButton::GPIOButton(QObject *parent) : QObject(parent)
{

    buttons[BTN_RFSTARTSTOP].gpio = gpioTools::GPIO_3;
    buttons[BTN_RFSTARTSTOP].pin = 23;
    buttons[BTN_RFSTARTSTOP].debounce = 10;
    buttons[BTN_RFSTARTSTOP].prevState = BTN_PRESSED;

    buttons[BTN_FOOTSWITCH].gpio = gpioTools::GPIO_5;
    buttons[BTN_FOOTSWITCH].pin = 20;
    buttons[BTN_FOOTSWITCH].debounce = 10;
    buttons[BTN_FOOTSWITCH].prevState = BTN_PRESSED;


    connect(&timer, SIGNAL(timeout()), this, SLOT(loop()));
    timer.setInterval(10);
    timer.start();
    keyElapsed.start();
    Button_state = BTN_STATE_RFSTART;
}

GPIOButton::~GPIOButton()
{
}

void GPIOButton::readBtnStatus(uint32 id)
{
#ifdef Button_Released
    buttons[id].currentState = gpioTools::ReadGPIOPort(buttons[id].gpio, buttons[id].pin); //struct
    if(buttons[id].currentState != buttons[id].prevState)
    {
        mdelay(buttons[id].debounce);
        buttons[id].currentState = gpioTools::ReadGPIOPort(buttons[id].gpio, buttons[id].pin);
        if(buttons[id].currentState == BTN_PRESSED)
        {
            keyElapsed.restart();
            qDebug("id = %d , Pressd" , id);

        }

        else if(buttons[id].currentState == BTN_NOT_PRESSED)
        {
            if((keyElapsed.elapsed() > 100) && !(keyElapsed.elapsed() >= 500))
            {
                emit sig_btnReleased(id);
                qDebug("id = %d , sig_Released" , id);
            }
            else if (keyElapsed.elapsed() >= 500)
            {
                emit sig_btnLongPressed(id);
                qDebug("id = %d , sig_LongPressed" , id);

            }
        }
       buttons[id].prevState = buttons[id].currentState;
    }

#endif
    buttons[id].currentState = gpioTools::ReadGPIOPort(buttons[id].gpio, buttons[id].pin); //struct
    if(buttons[id].currentState != buttons[id].prevState)
    {
        mdelay(buttons[id].debounce);
        buttons[id].currentState = gpioTools::ReadGPIOPort(buttons[id].gpio, buttons[id].pin);
        if(buttons[id].currentState == BTN_NOT_PRESSED)
        {

            keyElapsed.restart();

            //Button_state = BTN_STATE_RFSTART;
           //emit sig_btnReleased(id);
        }
        else if(buttons[id].currentState == BTN_PRESSED)
        {
            /*
            if((keyElapsed.elapsed() > 100) && !(keyElapsed.elapsed() >= RFSTARTBUTTON_TIME_MS)) //1s
            {
                emit sig_btnReleased(id);
            }
            //Button_state = BTN_STATE_RFSTOP;
            */
            if(Button_state == BTN_STATE_RFREADY)
            {
                Button_state = BTN_STATE_RFSTART;
            }
        }
       buttons[id].prevState = buttons[id].currentState;
    }
    if(Button_state == BTN_STATE_RFSTART)
    {
        if ((keyElapsed.elapsed() >= RFSTARTBUTTON_TIME_MS)&&(keyElapsed.elapsed() <= RFSTARTBUTTON_TIME_MS+100)&&(buttons[id].currentState == BTN_NOT_PRESSED)) //1s
        {
            //keyElapsed.restart();
            Button_state = BTN_STATE_RFSTOP;
            emit sig_btnLongPressed();
        }
    }
    else if(Button_state == BTN_STATE_RFSTOP)
    {
        if((keyElapsed.elapsed() > 100) && !(keyElapsed.elapsed() >= RFSTARTBUTTON_TIME_MS)) //1s
        {
            Button_state = BTN_STATE_RFREADY;
            emit sig_btnShortPressed();
        }
    }

}

void GPIOButton::loop()
{
    for(uint32 i ; i <BTN_MAX ; i ++ )
    {
        readBtnStatus(i);

    }
}

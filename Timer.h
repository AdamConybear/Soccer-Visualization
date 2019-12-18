#include <iostream>
#include <sstream>
#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
using namespace sf;
using namespace std;

class Timer{
public:
    /*
    @brief Constructor
    
    @param initialTime Initial time elapsed
    */
    Timer(Time initialTime = Time::Zero)
    {
        reset();
        add(initialTime);
    }

    /*
    @brief Add some time
    
    @param time Time to be added to the time elapsed
    @return Time elapsed
    */
    Time add(Time time)
    {
        m_time += time;

        if (m_state == STOPPED) m_state = PAUSED;

        return getElapsedTime();
    }

    /*
    @brief Reset the chronometer
    
    @param start if true the chronometer automatically starts
    @return Time elapsed on the chronometer before the reset
    */
    Time reset(bool start = false)
    {
        Time time = getElapsedTime();

        m_time = Time::Zero;
        m_state = STOPPED;

        if (start) resume();

        return time;
    }

    /*
    @brief Pause the chronometer
    
    @return Time elapsed

    @see toggle
    */
    Time pause()
    {
        if (isRunning())
        {
            m_state = PAUSED;
            m_time += m_clock.getElapsedTime();
        }
        return getElapsedTime();
    }

    /*
    @brief Resume the chronometer
    
    @return Time elapsed
    
    @see toggle
    */
    Time resume(){
        if (!isRunning()){ // if the clock is not running, start it back up again
            m_state = RUNNING;
            m_clock.restart();
        }
        return getElapsedTime();
    }

    /*
    @brief Pause or resume the chronometer
    
    If the chronometer is running the it is paused;
    otherwise it is resumes.
    
    @return Time elapsed

    @see pause
    @see resume
    */
    Time toggle(){
        if (isRunning()){
            pause();
        }else{
            resume();
        }

        return getElapsedTime();
    }

    /*
    @brief Tell the chronometer is running or not

    @brief chronometer's status
    */
    bool isRunning() const
    {
        return m_state == RUNNING;
    }

    /*
    @brief Give the amount of time elapsed since the chronometer was started
    
    @return Time elapsed
    */
    Time getElapsedTime() const
    {
        switch (m_state) {
            case STOPPED:
                return Time::Zero;

            case RUNNING:
                return m_time + m_clock.getElapsedTime();

            case PAUSED:
                return m_time;
        }
    }

    /*
    @brief Implicit conversion to sf::Time

    @return Time elapsed
    
    @see getElapsedTime
    */
    operator Time() const
    {
        return getElapsedTime();
    }

private:
    enum { STOPPED, RUNNING, PAUSED } m_state;  //!< state
    Time m_time;                            //!< time counter
    Clock m_clock;                          //!< clock
};
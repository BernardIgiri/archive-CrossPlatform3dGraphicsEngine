#pragma once


namespace renwin
{
    namespace util
    {
        // calls glfwInit on construction and glfwTerminate on destruction
        class GLFWInstance
        {
            public:
            GLFWInstance() { glfwInit(); };
            ~GLFWInstance() { glfwTerminate(); };
        };
        template <typename ListenerT,typename TriggerFuncT>
        class Delegate
        {
        public:
            typedef TriggerFuncT tiggerFunc_t; // trigger function type
            typedef boost::shared_ptr<ListenerT> listenerP_t; // function pointer type
            typedef boost::weak_ptr<ListenerT> listenerWkP_t; // weak function pointer type
            typedef ListenerT listener_t; // listener type
            // connects a function pointer to the delegate
            inline void Connect(listenerP_t func) { boost::mutex::scoped_lock lock(mutex);functions.push_back(listenerWkP_t(func)); };

            // triggers all objects in functions vector
            void Trigger(TriggerFuncT &trigger)
            {
                boost::mutex::scoped_lock lock(mutex); // lock access mutex
                typedef typename std::vector<listenerWkP_t>::iterator it_t;
                listenerP_t p;
                for (it_t i=functions.begin();i!=functions.end();i++)
                {
                    // copy to shared_ptr
                    p=i->lock();
                    // if not expired then trigger
                    if (!i->expired())
                        trigger(p);
                    else // elsewise remove from functions list
                        functions.erase(i);
                }
            }
        private:
            boost::mutex mutex; // access mutex
            std::vector<listenerWkP_t> functions; // list of functions
        };
        class ResChangeListener
        {
        public:
            ResChangeListener() : isChanged(false) {};
            void operator()(const math::vector2i &resolution_)
            {
                boost::mutex::scoped_lock lock(mutex);
                isChanged=true;
                resolution=resolution_;
            };
            bool IsChanged() { return isChanged; };
            math::vector2i Resolution() { boost::mutex::scoped_lock lock(mutex);return resolution; };
        private:
            volatile bool isChanged; // is changed flag
            math::vector2i resolution; // screen resolution
            boost::mutex mutex; // access mutex
        };
        class ChangeListener
        {
        public:
            ChangeListener() : isChanged(false) {};
            void operator()()
            {
                isChanged=true;
            };
            bool IsChanged() { return isChanged; };
        private:
            volatile bool isChanged; // is changed flag
        };
        template <typename TriggerT>
        typename TriggerT::listenerP_t ConnectTrigger(TriggerT &trigger)
        {
            typename TriggerT::listenerP_t func(new typename TriggerT::listener_t());
            trigger.Connect(func);
            return func;
        }
        class ChangeTrigger
        {
        public:
            // delegate type
            typedef util::Delegate<util::ChangeListener,ChangeTrigger> delegate_t;
            // function pointer type
            typedef delegate_t::listenerP_t listenerP_t;
            // function type
            typedef delegate_t::listener_t listener_t;

            void operator () (listenerP_t func) { func->operator()(); };
        };
    };
    class ResChangeTrigger
    {
    public:
        // delegate type
        typedef util::Delegate<util::ResChangeListener,ResChangeTrigger> delegate_t;
        // function pointer type
        typedef delegate_t::listenerP_t listenerP_t;
        // function type
        typedef delegate_t::listener_t listener_t;

        ResChangeTrigger(const math::vector2i &resolution_) : resolution(resolution_) {};
        void operator () (listenerP_t func)
        { func->operator()(resolution); };
    private:
        math::vector2i resolution; // new resolution
    };
    class Window
    {
    public:
        typedef ResChangeTrigger::listenerP_t       resChangeLsnr_t;
        typedef util::ChangeTrigger::listenerP_t    quitLsnr_t;
        // constructs window
        Window(const std::string &appTitle,math::vector2i &resolution,bool isFullscreen);
        // sets the window dimensions
        void SetResolution(const math::vector2i &resolution);
        // swaps the display buffers
        void SwapBuffers() const { glfwSwapBuffers(); };
        // issues request to close window
        void Quit() { boost::mutex::scoped_lock lock(mutex);util::ChangeTrigger t;quitEvent.Trigger(t); };
        // get an on reschange listener
        resChangeLsnr_t OnResChange() { return ConnectTrigger(resChangeEvent); };
        // get an on quit listener
        quitLsnr_t OnQuit() { return ConnectTrigger(quitEvent); };
    private:
        ResChangeTrigger::delegate_t    resChangeEvent;
        util::ChangeTrigger::delegate_t quitEvent;
        boost::mutex mutex; // thread mutex for window
        util::GLFWInstance glfwIns; // glfw Instance
    };
}

#ifndef OTHELLO_APP_HPP
#define OTHELLO_APP_HPP



namespace othello
{
    
    ////////////////////////////////////////////////////////////////
    /// \class App
    ///
    /// \brief Class that starts and runs the application
    ///
    ////////////////////////////////////////////////////////////////
    class App
    {

        public:
        
            ////////////////////////////////////////////////////////////////
            /// \brief Static function to start and run the program using
            ///        the options from argc and argv
            ///
            /// \param argc argc from `main`
            /// \param argv argv from `main`
            ///
            /// \return A code representing the success of the program.
            ///         Should be returned from `main`
            ///
            ////////////////////////////////////////////////////////////////
            static int run(const int& argc, char** argv);
            
    };
    
}

#endif //OTHELLO_APP_HPP

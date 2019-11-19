// % Copyright (c) 2011, Chris Marsh
// 	% All rights reserved.
// 	% 
// 	% Redistribution and use in source and binary forms, with or without 
// 	% modification, are permitted provided that the following conditions are 
// 	% met:
// % 
// 	%     * Redistributions of source code must retain the above copyright 
// 	%       notice, this list of conditions and the following disclaimer.
// 	%     * Redistributions in binary form must reproduce the above copyright 
// 	%       notice, this list of conditions and the following disclaimer in 
// 	%       the documentation and/or other materials provided with the distribution
// 	%     * Neither the name of the University of Saskatchewan nor the names 
// 	%       of its contributors may be used to endorse or promote products derived 
// 	%       from this software without specific prior written permission.
// 	%       
// 	% THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
// 	% AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
// 	% IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
// 	% ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE 
// 	% LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
// 	% CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
// 	% SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
// 	% INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
// 	% CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
// 	% ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
// 	% POSSIBILITY OF SUCH DAMAGE.


#include "matlab_engine.h"
#include "exception.hpp"

namespace maw
{

    void matlab_engine::start()
    {
        if (!(m_engine = engOpen("\0")))
        {
            throw std::runtime_error("Can't start MATLAB engine");
        }
    }

    void matlab_engine::stop()
    {
        if (m_engine)
        {
            if (engEvalString(m_engine, "close") == 1)
                throw std::runtime_error(get_last_error().c_str());
        }
        m_engine = NULL;
    }

    void matlab_engine::evaluate(std::string command)
    {
        if (m_engine)
        {
            engEvalString(m_engine, "lasterror('reset')");
            engEvalString(m_engine, command.c_str());

            std::string err = get_last_error().c_str();
            if (err != "")
                throw std::runtime_error((std::string(__FILE__) + std::string(":") + boost::lexical_cast<std::string, int>(__LINE__) + std::string(":") + get_last_error() + std::string("\nCommand: ") + command).c_str());
        } else
        {
            throw std::runtime_error("No MATLAB engine open");
        }
    }

    void matlab_engine::put(std::string name, mxArray* var)
    {
        if (m_engine)
        {
            if (engPutVariable(m_engine, name.c_str(), var) == 1)
                throw std::runtime_error(get_last_error().c_str());
        } else
        {
            throw std::runtime_error("No MATLAB engine open");
        }
    }

    mxArray* matlab_engine::get(std::string name)
    {

        if (m_engine)
        {
            mxArray* temp = engGetVariable(m_engine, name.c_str());
            if (!temp)
                throw std::runtime_error(get_last_error().c_str());
            else
                return temp;

        } else
        {
            throw std::runtime_error("No MATLAB engine open");
        }
    }

    matlab_engine::~matlab_engine()
    {
        LOG_DEBUG << "Closing Matlab instance";
        if (m_engine)
        {
            engEvalString(m_engine, "close");
        }
        m_engine = NULL;
    }

    matlab_engine::matlab_engine()
    {
        m_engine = NULL;
    }

    void matlab_engine::set_working_dir(std::string dir)
    {
        if (m_engine)
        {
            if (engEvalString(m_engine, (std::string("cd '") + dir + std::string("'")).c_str()) == 1)
                throw std::runtime_error(get_last_error().c_str());
        } else
        {
            throw std::runtime_error("No MATLAB engine open");
        }
    }

    void matlab_engine::set_working_dir()
    {
        char path[FILENAME_MAX];
        char* ret = GetCurrentDir(path, FILENAME_MAX);
        if (!ret)
            BOOST_THROW_EXCEPTION(matlab_null_return() << errstr_info("Unable to set path"));
        
        set_working_dir(std::string(path));

    }

    std::string matlab_engine::get_last_error()
    {
        if (m_engine)
        {
            int retval = engEvalString(m_engine, "myErr=lasterror");
            if (retval == 1)
                BOOST_THROW_EXCEPTION(matlab_engine_failure() << errstr_info("Catastrophic matlab engine failure"));
            // get the struct
            mxArray *err = engGetVariable(m_engine, "myErr");
            char str[512];
            if (err && mxIsStruct(err))
            {
                // get the error message string field
                mxArray *errStr = mxGetField(err, 0, "message");
                if ((errStr != NULL) && mxIsChar(errStr))
                {
                    // get the string
                   retval = mxGetString(errStr, str, sizeof (str) / sizeof (str[0]));
                   if (retval == 1)
                        BOOST_THROW_EXCEPTION(matlab_engine_failure() << errstr_info("Catastrophic matlab engine failure"));
                }
            }
            mxDestroyArray(err);
            err = NULL;
            return std::string(str);
        } else
        {
            return "";
        }
    }

    void matlab_engine::put_scalar(std::string name, double var)
    {
        mxArray* mx = mxCreateDoubleMatrix(1,1, mxREAL);
        memcpy(mxGetPr(mx), &var, sizeof (double));
        put(name, mx);
        mxDestroyArray(mx);
    }

    void matlab_engine::put_double_matrix(std::string name, const d_mat mat)
    {
        mxArray* mx = mxCreateDoubleMatrix(mat->n_rows, mat->n_cols, mxREAL);
        memcpy(mxGetPr(mx), mat->memptr(), mat->n_elem * sizeof (double));
        put(name, mx);
        mxDestroyArray(mx);
    }

    d_mat matlab_engine::get_double_matrix(std::string name)
    {
        mxArray* mx = get(name);

        if (!mx)
        {
            d_mat(); //"null" ptr
        }

        d_mat out_matrix(new arma::mat(mxGetM(mx), mxGetN(mx)));

        memcpy(out_matrix->memptr(), mxGetPr(mx), out_matrix->n_elem * sizeof (double));

        mxDestroyArray(mx);

        return out_matrix;

    }

    d_vec matlab_engine::get_double_vector(std::string name)
    {
        mxArray* mx = get(name);

        if (!mx)
        {
            return d_vec(); //"null" ptr
        }

        size_t M = mxGetM(mx);

        d_vec out_vec(new arma::vec(M));


        memcpy(out_vec->memptr(), mxGetPr(mx), out_vec->n_elem * sizeof (double));

        mxDestroyArray(mx);

        return out_vec;

    }

    double matlab_engine::get_scaler(std::string name)
    {
        if (m_engine)
        {
            mxArray* mx =  thi(*s)[name];
            double d = mxGetScalar(mx);
            mxDestroyArray(mx);
            return d;
        }
        return 0.0;


    }

    void matlab_engine::put_double_vector(std::string name, const d_vec vec)
    {
        mxArray* mx = mxCreateDoubleMatrix(vec->n_rows, 1, mxREAL);
        memcpy(mxGetPr(mx), vec->memptr(), vec->n_elem * sizeof (double));
        put(name, mx);
        mxDestroyArray(mx);
    }

    void matlab_engine::add_dir_to_path(std::string dir)
    {
        if (m_engine)
        {
            if (boost::filesystem::exists(dir))
            {
                if (engEvalString(m_engine, (std::string("addpath('") + dir + std::string("')")).c_str()) == 1)
                    throw std::runtime_error(get_last_error().c_str());
                LOG_DEBUG << "Added path to engine: " << dir; 
            }
            else
            {
                LOG_WARNING << "Path " << dir << " does not exist. Not added.";
            }
        } else
        {
            throw std::runtime_error("No MATLAB engine open");
        }
    }

}
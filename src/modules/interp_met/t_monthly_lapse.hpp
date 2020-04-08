//
// Canadian Hydrological Model - The Canadian Hydrological Model (CHM) is a novel
// modular unstructured mesh based approach for hydrological modelling
// Copyright (C) 2018 Christopher Marsh
//
// This file is part of Canadian Hydrological Model.
//
// Canadian Hydrological Model is free software: you can redistribute it and/or
// modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Canadian Hydrological Model is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Canadian Hydrological Model.  If not, see
// <http://www.gnu.org/licenses/>.
//

#pragma once

#include "../logger.hpp"
#include "triangulation.hpp"
#include "module_base.hpp"
#include <cstdlib>
#include <string>

#include <cmath>

#include <math.h>

/**
* \addtogroup modules
* @{
* \class t_monthly_lapse
* \brief Constant monthly linear lapse rate adjustment
*
* Constant monthly linear lapse rate adjustment for air temperature.
* Taken as input from user or default values for Canadian Rockies from:
 * *
* Joseph M. Shea, Shawn J. Marshall and Joanne M. Livingston
* Arctic, Antarctic, and Alpine Research
* Vol. 36, No. 2 (May, 2004), pp. 272-279
*
* Depends:
* - None
*
* Provides:
* - Air temperature "t" [degC]
* - Air temperatue "t_monthly_lapse" [degC]
*

*/
class t_monthly_lapse final : public module_base
{
REGISTER_MODULE_HPP(t_monthly_lapse);
public:
    t_monthly_lapse(config_file cfg);
    ~t_monthly_lapse();
    virtual void run(mesh_elem& face);
    virtual void init(mesh& domain);
    struct data : public face_info
    {
        interpolation interp;
    };
    double MLR[12];
};

/**
@}
*/

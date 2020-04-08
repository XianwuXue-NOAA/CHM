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
* \class Cullen_monthly_llra_ta
* \brief Constant monthly linear lapse rate adjustment
*
* Constant monthly linear lapse rate adjustment for air temperature.
*
* Depends:
* - None
*
* Provides:
* - Air temperature "t" [degC]

*
* Reference:
* >Cullen, R. M., and S. J. Marshall (2011), Mesoscale temperature patterns in the Rocky Mountains and foothills region of southern Alberta, Atmos. - Ocean, 49(3), 189–205, doi:10.1080/07055900.2011.592130.
*/
class Cullen_monthly_llra_ta final : public module_base
{
REGISTER_MODULE_HPP(Cullen_monthly_llra_ta);
public:
    Cullen_monthly_llra_ta(config_file cfg);
    ~Cullen_monthly_llra_ta();
    virtual void run(mesh_elem& face);
    virtual void init(mesh& domain);
    struct data : public face_info
    {
        interpolation interp;
    };
};

/**
@}
*/

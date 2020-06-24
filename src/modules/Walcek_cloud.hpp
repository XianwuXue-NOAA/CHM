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

#include "module_base.hpp"
#include <math.h>
#include <algorithm>


#include <meteoio/MeteoIO.h>
/**
 * \ingroup modules atm iswr lw
 * @{
 * \class Walcek_cloud
 *
 * Calculates a cloud fraction using 700mvb RH. Extrapolates RH@700mb via Kunkel (1989).
 *
 * **Depends:**
 * - Air temperature "t" [ \f$ {}^\circ C \f$ ]
 * - Relative humidity "rh" [%]
 *
 * **Provides:**
 * - Atmospheric transmittance "cloud_frac" [0,1]
 *
 * **References:**
 * - Walcek, C. J. (1994). Cloud cover and its relationship to relative humidity during a springtime midlatitude cyclone.
 * Monthly Weather Review, 122(6), 1021–1035.
 * - Kunkel, K. E. (1989). Simple procedures for extrapolation of humidity variables in the mountainous western United States.
 * Journal of Climate, 2(7), 656–669.
 *
 * @}
*/
class Walcek_cloud : public module_base
{
REGISTER_MODULE_HPP(Walcek_cloud);
public:
    Walcek_cloud(config_file cfg);
    ~Walcek_cloud();
    virtual void run(mesh_elem& face);

};

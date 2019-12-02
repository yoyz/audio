/* Copyright 2013-2017 Matt Tytel
 *
 * mopo is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * mopo is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with mopo.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once
#ifndef TWITCHHELM_GATE_H
#define TWITCHHELM_GATE_H

#include "twytchhelm_value.h"

namespace twytchhelmmopo {

  class Gate : public Processor {
    public:
      enum Inputs {
        kChoice,
        kNumInputs
      };

      Gate();

      virtual void destroy() override;
      virtual Processor* clone() const override { return new Gate(*this); }
      void process() override;

    private:
      void setSource(int source);

      mopo_float* original_buffer_;
  };
} // namespace twytchhelmmopo

#endif // GATE_H

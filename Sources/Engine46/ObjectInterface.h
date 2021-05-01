/**
 * @file ObjectInterface.h
 * @brief
 * @author 木村優
 * @date 2021/05/01
 */

#pragma once

#include "DataRecord.h"

namespace Engine46 {

	class ObjectInterface {
	protected:
		std::vector<DATARECORD>		vecDataRecord;
		std::vector<STR_DATARECORD>	vecStrDataRecord;

		virtual void Initialize() = 0;
		virtual void Update() = 0;
		virtual void Draw() = 0;

		virtual bool Save(std::ofstream& ofs) = 0;
		virtual bool Load(std::ifstream& ifs) = 0;
	};

} // namespace

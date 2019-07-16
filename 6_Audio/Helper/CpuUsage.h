/*
 * CpuUsage.h
 *
 *  Created on: Jul 10, 2019
 *      Author: vqtrong
 */

#ifndef UTILS_CPUUSAGE_H_
#define UTILS_CPUUSAGE_H_

#pragma once

#include <stdint.h>

class CpuUsage
{
public:
    static CpuUsage& getIntance()
    {
        static CpuUsage __intance;
        return __intance;
    }

    uint32_t getCurrentUsage();

private:
    uint32_t _cpu_cycle_per_tick;

    CpuUsage();
    void initCpuUsage();
};

#endif /* UTILS_CPUUSAGE_H_ */

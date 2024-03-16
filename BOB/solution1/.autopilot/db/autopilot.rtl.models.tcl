set SynModuleInfo {
  {SRCNAME divider MODELNAME divider RTLNAME divider IS_TOP 1
    SUBMODULES {
      {MODELNAME divider_mul_32ns_32s_64_1_1 RTLNAME divider_mul_32ns_32s_64_1_1 BINDTYPE op TYPE mul IMPL auto LATENCY 0 ALLOW_PRAGMA 1}
      {MODELNAME divider_mul_32s_32s_32_1_1 RTLNAME divider_mul_32s_32s_32_1_1 BINDTYPE op TYPE mul IMPL auto LATENCY 0 ALLOW_PRAGMA 1}
      {MODELNAME divider_mul_64s_16ns_80_1_1 RTLNAME divider_mul_64s_16ns_80_1_1 BINDTYPE op TYPE mul IMPL auto LATENCY 0 ALLOW_PRAGMA 1}
      {MODELNAME divider_divLUT_ROM_AUTO_1R RTLNAME divider_divLUT_ROM_AUTO_1R BINDTYPE storage TYPE rom IMPL auto LATENCY 2 ALLOW_PRAGMA 1}
    }
  }
}

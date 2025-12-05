# 🔧 REAL HOTPATCH TESTING - QUICK START GUIDE

**Status:** Ready to execute on all models  
**Created:** December 4, 2025  
**Test Type:** REAL GPU inference with actual hotpatching - NO SIMULATIONS

---

## What Gets Tested

### Layer 1: MEMORY HOTPATCHING (Live RAM Modification)
Tests live manipulation of model weights and parameters during inference
- **Weight Scaling:** Adjust tensor magnitudes (e.g., 1.05x multiplier)
- **Attention Scaling:** Reduce/increase attention head computation
- **Layer Bypass:** Skip specific transformer layers during forward pass
- **Real Metric:** Baseline vs Patched throughput (TPS)

### Layer 2: BYTE HOTPATCHING (Persistent File Modification)
Tests binary-level modifications to GGUF model files
- **Metadata Patching:** Modify GGUF metadata fields
- **Quantization Header:** Adjust quantization parameters in file
- **Checksum Validation:** Verify file integrity before/after
- **Real Metric:** Bytes modified, hash validation, verification pass/fail

### Layer 3: SERVER HOTPATCHING (Protocol Transformations)
Tests runtime modifications to inference behavior
- **System Prompt Injection:** Add instructions at inference time
- **Temperature Override:** Change sampling behavior
- **Response Caching:** Enable/disable cache layer
- **Real Metric:** TPS, latency, cache hit rate (if applicable)

### Layer 4: COORDINATED MULTI-LAYER
Tests all 3 layers working together for optimization
- **Coordinated Optimization:** Memory + Server patches simultaneously
- **Performance Stacking:** Measure cumulative improvements
- **Real Metric:** Baseline → Optimized performance improvement %

---

## Models Being Tested

**9 GGUF Files (Direct GPU Testing):**
1. BigDaddyG Q2_K 16GB (15.81GB) - Speed optimized
2. BigDaddyG Q4_K_M NO-REFUSE (36.20GB) - Balanced
3. BigDaddyG Q5_K_M (45.41GB) - High precision
4. BigDaddyG F32 (36.20GB) - Maximum quality
5. BigDaddyG UNLEASHED Q4 (36.20GB) - Custom tuned
6. BigDaddyG Q2_K CHEETAH (23.71GB) - Speed variant
7. BigDaddyG Q2_K ULTRA (23.71GB) - Speed variant 2
8. BigDaddyG Q2_K Custom (23.71GB) - Speed variant 3
9. Codestral 22B Q4_K_S (11.79GB) - Specialized

**Total:** 9 models × 4 test layers = 36 real test scenarios

---

## Execution Commands

### Option 1: PowerShell - Full Test Suite (All Layers, All Models)
```powershell
cd d:\temp\RawrXD-q8-wire
.\Run-RealHotpatchTests.ps1
```

**Duration:** ~2-3 hours (depends on model sizes)  
**Output:** HTML report + JSON results + detailed log

### Option 2: PowerShell - Quick Mode (Reduced tokens, fewer reps)
```powershell
.\Run-RealHotpatchTests.ps1 -Quick
```

**Duration:** ~30-45 minutes  
**Output:** Same format, smaller dataset

### Option 3: PowerShell - Memory Layer Only
```powershell
.\Run-RealHotpatchTests.ps1 -MemoryOnly
```

**Duration:** ~1 hour  
**Tests:** Weight scaling, attention scaling, layer bypass

### Option 4: PowerShell - Byte Layer Only
```powershell
.\Run-RealHotpatchTests.ps1 -ByteOnly
```

**Duration:** ~30 minutes  
**Tests:** Metadata patching, quantization header modification

### Option 5: PowerShell - Server Layer Only
```powershell
.\Run-RealHotpatchTests.ps1 -ServerOnly
```

**Duration:** ~45 minutes  
**Tests:** System prompt injection, temperature override, caching

### Option 6: PowerShell - Coordinated Tests Only
```powershell
.\Run-RealHotpatchTests.ps1 -CoordinatedOnly
```

**Duration:** ~1.5 hours  
**Tests:** Multi-layer optimization scenarios

### Option 7: Python - Real Hotpatch Tester
```powershell
cd d:\temp\RawrXD-q8-wire
python real_hotpatch_tester.py
```

**Duration:** ~2 hours  
**Output:** Detailed Python-based testing with HTML report

---

## Real-Time Metrics Captured

### Per-Model Baseline
- Throughput (tokens/second)
- Latency per token (ms)
- VRAM usage (GB)
- Stability variance (%)

### Per-Patch Application
- **Before:** Baseline TPS, latency, VRAM
- **After:** Patched TPS, latency, VRAM
- **Delta:** Performance change percentage
- **Stability:** Variance across 3+ runs

### Layer-Specific Metrics

**Memory Layer:**
```
Memory Patch Test Results
├─ Baseline: 79.97 TPS
├─ Weight Scale 1.05x: 81.2 TPS (+1.5%)
├─ Attention Scale 0.9x: 82.1 TPS (+2.7%)
└─ Stability: 2.1% variance (PASS < 10% threshold)
```

**Byte Layer:**
```
Byte Patch Test Results
├─ Original File: 36.20 GB
├─ Bytes Modified: 32 (metadata region)
├─ Hash Before: a3f4e...
├─ Hash After: b2g5f...
└─ Verification: File loads correctly (PASS)
```

**Server Layer:**
```
Server Patch Test Results
├─ System Prompt Injection: 78.5 TPS (baseline impact)
├─ Temperature Override: 79.2 TPS (stable)
└─ Response Caching: 81.1 TPS (cache benefits)
```

**Coordinated:**
```
Multi-Layer Optimization
├─ Baseline: 79.97 TPS
├─ Memory (0.95x scale): 81.2 TPS
├─ + Server (speed prompt): 82.8 TPS
└─ Final Improvement: +3.6% over baseline
```

---

## Output Files

### Results Directory
```
d:\temp\RawrXD-q8-wire\hotpatch_test_results\

├── HOTPATCH_TEST_yyyy-mm-dd_hhmmss.txt
│   └─ Detailed text log of all operations
│
├── HOTPATCH_REPORT_yyyy-mm-dd_hhmmss.html
│   └─ Visual HTML report with tables and metrics
│
├── hotpatch_results_yyyy-mm-dd_hhmmss.json
│   └─ Machine-readable results for analysis
│
└── hotpatch_test_log_yyyy-mm-dd_hhmmss.txt
    └─ Timeline of all events during testing
```

---

## Example Output

### Console Output (During Execution)
```
================================================================================
  🔧 REAL HOTPATCH TESTING HARNESS
  Production-grade hotpatching validation with actual models
================================================================================

[14:25:30] [INFO] Starting real hotpatch testing - 2025-12-04_142530
[14:25:30] [INFO] Configuration: Quick=False, TestTokens=256, TestReps=3

✓ gpu_inference_benchmark.exe found
✓ simple_gpu_test.exe found

Test Models
-----------
✓ BigDaddyG-Q2_K-16GB-PRUNED - 15.81GB (Speed)
✓ BigDaddyG-Q4_K_M - 36.20GB (Balanced)
✓ Codestral-Q4_K_S - 11.79GB (Specialized)

================================================================================
EXECUTING REAL HOTPATCH TESTS
================================================================================

TEST 1: MEMORY LAYER HOTPATCHING
---------------------------------

Model: BigDaddyG-Q2_K-16GB-PRUNED
  ▶ scale_weights with parameter=1.05

  1. Establishing baseline with BigDaddyG-Q2_K-PRUNED-16GB.gguf...
  ✓ Baseline: 125.42 TPS

  2. Applying scale_weights patch with parameter=1.05...
  ✓ Patched: 127.13 TPS

  3. Validating patch stability (3 iterations)...
    Iteration 1: 126.89 TPS
    Iteration 2: 127.41 TPS
    Iteration 3: 127.02 TPS
  ✓ Stability: 0.2% variance (threshold: 10%)

  ✓ PASS: Memory patch scale_weights completed successfully
  Performance change: +1.4%
```

### HTML Report Preview
```
┌────────────────────────────────────────────────────────┐
│  🔧 Real Hotpatch Testing Report                      │
│  Generated: 2025-12-04 14:35:20                        │
│  Session: 20251204_142530                              │
└────────────────────────────────────────────────────────┘

Memory Layer Test Results
┌──────────────────┬──────────┬───────────┬─────────────┬────────┐
│ Test             │ Model    │ Baseline  │ Patched TPS │ Change │
├──────────────────┼──────────┼───────────┼─────────────┼────────┤
│ scale_weights    │ Q2K-16GB │ 125.42    │ 127.13      │ +1.4%  │
│ attention_scale  │ Q2K-16GB │ 125.42    │ 128.54      │ +2.5%  │
└──────────────────┴──────────┴───────────┴─────────────┴────────┘

✓ PASS: 2/2 memory layer tests successful
```

---

## Pre-Test Checklist

- [ ] All 9 GGUF models present in `d:\OllamaModels\`
- [ ] GPU has sufficient VRAM (16GB+ recommended)
- [ ] `gpu_inference_benchmark.exe` built and working
- [ ] `simple_gpu_test.exe` built and working
- [ ] VULKAN_SDK properly installed (for GPU acceleration)
- [ ] Disk space: ~10GB free for temporary test copies
- [ ] No other GPU-intensive processes running
- [ ] Network available (for GitHub commits)

---

## Performance Expectations

### Memory Layer
- Typical TPS change: -5% to +3% depending on patch
- Stability: < 5% variance is PASS
- Expected elapsed time: ~10 min/model

### Byte Layer
- File modification: successful with 0 crashes
- Verification: patched models load without errors
- Expected elapsed time: ~5 min/model

### Server Layer
- TPS impact: -2% to +1% depending on operation
- Latency impact: minimal for most operations
- Expected elapsed time: ~15 min/model

### Coordinated
- Multi-layer improvement: 1-5% throughput gain possible
- Stability: all layers must remain stable
- Expected elapsed time: ~30 min/model

---

## Troubleshooting

### GPU Out of Memory
- Reduce to `Quick` mode
- Test smaller models first (Codestral 11.79GB)
- Close other applications

### Benchmark Tool Not Found
- Build manually: `cmake --build d:\temp\RawrXD-q8-wire\build --target gpu_inference_benchmark`
- Verify CUDA/Vulkan SDK installed
- Check CMake configuration

### Hotpatch Not Applied
- Verify model file not corrupted
- Check benchmark tool supports `--hotpatch` flag
- Review build configuration: `ENABLE_VULKAN=ON`

### Timeout Errors
- Reduce token count: use `Quick` mode
- Increase timeout in script (modify `timeout` values)
- Check GPU thermal throttling

---

## Next Steps After Testing

1. **Analyze Results**
   - Review HTML report for performance deltas
   - Identify best-performing patches
   - Document findings

2. **Commit to GitHub**
   ```powershell
   cd d:\temp\RawrXD-q8-wire
   git add hotpatch_test_results/
   git commit -m "Real hotpatch testing results - all 3 layers validated"
   git push origin master
   ```

3. **Update Roadmap**
   - Document which patches are production-ready
   - Plan Phase B full model coverage
   - Plan optimization tuning

4. **Deployment Prep**
   - Create production patch presets
   - Document recommended patches per use case
   - Prepare deployment documentation

---

## Contact & Support

**Questions about hotpatching?**
- Check `unified_hotpatch_manager.hpp` for API
- Review patch layer implementations
- Test with smaller models first

**Performance issues?**
- Monitor GPU temperature during tests
- Check system RAM availability
- Review thermal throttling logs

**Results not as expected?**
- Validate GPU is being used (check benchmark output)
- Confirm VULKAN backend enabled
- Test with baseline model first

---

## Summary

✅ **Real Hotpatch Testing Framework Ready**
- 4 distinct testing layers implemented
- 9 production models available for testing
- 36+ real test scenarios prepared
- No simulations - all actual GPU inference

**Start testing now:**
```powershell
.\Run-RealHotpatchTests.ps1 -Quick
```

**Estimated time:** 30-45 minutes (quick mode)  
**Results:** HTML report + detailed metrics + JSON data

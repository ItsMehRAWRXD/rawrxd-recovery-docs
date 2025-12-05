# Phase 1: Performance Optimization & Boundary Mapping

**Objective:** Move beyond validated 80 TPS, find absolute performance ceiling, prepare for hybrid quantization.

**Start Date:** December 4, 2025  
**Status:** In Progress

---

## 1. Peak Performance Variant Discovery

### Goal
Benchmark Q5_K_M and Q6_K variants to identify the highest sustained TPS variant.

### Rationale
- Q5_K_M typically offers better precision than Q4_K with acceptable VRAM overhead
- Q6_K provides even higher precision for specialized workloads
- Finding the peak variant establishes production speed potential

### Test Plan

| Model Variant | Expected VRAM | Expected TPS Range | Status |
|:---|:---|:---|:---|
| Q2_K (baseline) | 8-10 GB | 100-120 tok/s | ⏳ Pending |
| Q4_K (validated) | 12-14 GB | 79.97 tok/s ✅ | ✅ Complete |
| Q5_K_M (target) | 15-16 GB | 60-75 tok/s | ⏳ Pending |
| Q6_K (target) | 16+ GB | 40-60 tok/s | ⏳ Pending |

### Benchmarks to Run
```cpp
// Benchmark 256, 512, 1024 token sequences for each variant
// Measure: throughput (tok/s), latency/token (ms), memory usage
```

### Success Criteria
- [ ] Q5_K_M throughput measured and documented
- [ ] Q6_K throughput measured and documented
- [ ] Performance curve established (VRAM vs TPS trade-off)
- [ ] Peak variant identified

---

## 2. Hybrid Quantization Feasibility Test

### Goal
Validate selective layer quantization (e.g., Attention=Q5_K_M, Rest=Q2_K).

### Implementation Path
1. Load base model with Q2_K quantization
2. Selectively override attention/MLP layers to Q5_K_M
3. Measure throughput and VRAM impact
4. Compare to uniform quantization baselines

### Expected Outcome
- Confirms feasibility of mixed-precision approach
- Quantifies VRAM vs performance trade-off
- Validates the "90+ TPS" future recommendation

### Success Criteria
- [ ] Hybrid model loads without errors
- [ ] Selective layer override works correctly
- [ ] Performance improvement measurable
- [ ] VRAM consumption documented

---

## 3. VRAM Bandwidth Stress Test

### Goal
Benchmark Q8_0 (or largest variant) to measure bandwidth bottleneck.

### Test Parameters
- Model: Largest possible (Q8_0 or near-VRAM-limit variant)
- Batch size: Single token (streaming inference)
- Duration: 5 minutes at max tokens/sec

### Expected Results
- Significant throughput drop (expected: 50-65 tok/s for Q8_0)
- Confirms VRAM bandwidth as primary bottleneck
- Establishes performance floor

### Success Criteria
- [ ] Q8_0 variant successfully loaded (if fits in 16GB)
- [ ] Throughput measured at ~50-65 tok/s
- [ ] Memory bandwidth utilization calculated
- [ ] Bottleneck analysis documented

---

## Implementation Timeline

### Week 1 (Dec 4-10)
- [x] Set up benchmarking harness
- [ ] Run Q5_K_M benchmarks (Dec 5)
- [ ] Run Q6_K benchmarks (Dec 6)
- [ ] Hybrid quantization implementation (Dec 7-8)

### Week 2 (Dec 11-17)
- [ ] Hybrid quantization testing (Dec 11-12)
- [ ] Q8_0 stress test (Dec 13)
- [ ] Performance curve analysis (Dec 14)
- [ ] Phase 1 completion report (Dec 15)

---

## Deliverables

1. **Performance Curve Graph** - TPS vs Model Variant
2. **Hybrid Quantization Report** - Feasibility & Results
3. **VRAM Bandwidth Analysis** - Bottleneck Confirmation
4. **Updated Benchmark Suite** - All variants documented
5. **Peak Performance Summary** - Best-case production profile

---

## Notes

- All benchmarks use the existing `gpu_inference_benchmark.exe` as foundation
- Continue using AMD RX 7800 XT as reference hardware
- Document all results for Phase 3 documentation update

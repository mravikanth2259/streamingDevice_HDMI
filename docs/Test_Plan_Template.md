# Test Plan Template: Portable HDMI Streaming Device

**Aligned to:** [Detailed_Design_Document.md](Detailed_Design_Document.md) §16 Testing Strategy and Plan

---

## 1. Test Plan Metadata

| Field | Value |
|-------|-------|
| **Product** | Portable HDMI Streaming Device |
| **Version under test** | |
| **Test plan owner** | |
| **Date** | |
| **DDD version** | 2.0 |

---

## 2. Test Scope

- [ ] Unit tests (HAL, mocks)
- [ ] Service tests (App Launcher, UI, Config, CEC, Codec, Container, Stream)
- [ ] Integration tests (I-001 to I-004)
- [ ] Hardware-in-the-loop (HDMI, CEC, BLE)
- [ ] Performance (latency, frame rate)
- [ ] Security (OTA signature, TLS)
- [ ] End-to-end user flows

---

## 3. Test Environment

| Item | Specification |
|------|---------------|
| **Build** | `make all` or `cmake --build` |
| **Test runner** | `make test` / `ctest` |
| **Mock drivers** | All HAL mocks enabled |
| **Reference media** | Licensed ProRes; synthetic H.265, AV1, VP9 |
| **HiL** | TV model(s): _______________ ; Remote: _______________ |

---

## 4. Test Cases (per DDD Traceability)

Map each Traceability Matrix requirement to test case IDs:

| Req ID | Test Case IDs | Pass/Fail |
|--------|---------------|-----------|
| US-001 | H-DIS-001 to H-DIS-004 | |
| US-002 | H-CEC-001, I-001 | |
| US-006 | H-APP-002, I-001 | |
| … | … | |

---

## 5. Pass/Fail Criteria

- **Unit/Service/Integration:** 100% pass
- **Performance:** 95th percentile input-to-action < 200 ms
- **Security:** OTA with invalid signature rejected
- **Accessibility:** Focus visible; full D-pad navigation

---

## 6. Defects and Issues

| ID | Severity | Description | Status |
|----|----------|-------------|--------|
| | | | |

---

## 7. Sign-Off

| Role | Name | Date |
|------|------|------|
| Test Lead | | |
| Product | | |
| Engineering | | |

---

*Template version: 1.0. See [Test_Suite_Document.md](Test_Suite_Document.md) for detailed test cases.*

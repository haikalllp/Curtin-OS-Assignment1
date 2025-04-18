Below is the **additional epic** you must tack on to ensure 100% compliance:

---

## 📄 Epic: Report PDF & Submission Packaging

- [ ] Download and include the **signed cover page** (declaration of originality) from Blackboard (PDF).  
- [ ] Write “**Discussion of shared data structures and synchronization**” section in the report:
  - [ ] List each shared variable (`A`, `swap_count`, `turn`, `no_swap_*`)  
  - [ ] Explain which threads access each and how you protect them (mutexes/conds)  
- [ ] Write “**Known limitations & incorrect cases**”:
  - [ ] Describe any input patterns or sizes that your program does not handle  
  - [ ] Explain how you tested to confirm correct behavior  
- [ ] Write “**Sample inputs & outputs**”:
  - [ ] Include at least the 15‑element example and its expected output  
  - [ ] Include edge cases (empty, single element, max‑200)  
  - [ ] For each: show the actual output and a brief correctness check  
- [ ] Append “**Full source code listing**” exactly as submitted under `src/` and `include/`.  
- [ ] Assemble these sections into one **PDF report** (≤ 6 pages of content as per unit outline).  
- [ ] Create a **ZIP** named `FirstName_FamilyName_ID.zip` containing:
  - [ ] `src/` (all `.c`)  
  - [ ] `include/` (all `.h`)  
  - [ ] `Makefile`  
  - [ ] `README.md` (build & run instructions, dependencies, design summary)  
  - [ ] `report.pdf`  
- [ ] Verify ZIP structure:
  - [ ] Top‑level contains exactly the above files/folders  
  - [ ] Building (`make`) and running (`./sss ToSort`) still works unmodified  
- [ ] Upload ZIP to Blackboard before 5 May 2025 under your student account  

---

With this epic tacked on, **every** requirement in the assignment brief is now covered—from code through report to submission.
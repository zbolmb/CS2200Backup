#include "tlb.h"
#include "pagetable.h"

/**
 * This function simulates a the TLB lookup, and uses the second chance algorithm
 * to evict an entry
 *
 * @param vpn The virtual page number that has to be translated
 * @param offset The page offset of the virtual address
 * @param rw Specifies if the access is a read or a write
 * @param stats The struct for statistics
 */
uint64_t tlb_lookup(uint64_t vpn, uint64_t offset, char rw, stats_t *stats)
{

    // (1) Look for the pfn in the TLB.
    // If you find it here
    // (2) update the frequency count of the page table entry using the
    //     current_pagetable global.
    // (3) Mark the TLB entry as used - for clock sweep
    // (4) Make sure to mark the entry dirty in the TLB if it is a write access
    // (5) return the PFN you just found in the TLB


    /********* TODO ************/
    // int k;
    // tlbe_t *cur1;
    // for (k = 0; k < tlb_size; k++) {
    //     cur1 = tlb + k;
    //         printf("tlb slot %"PRIx64" %"PRIx64" %"PRIx64"\n", cur1->vpn, cur1->pfn, current_pagetable[cur1->vpn].frequency);
    // }
    // printf("\n");

    stats->accesses++;
    if (rw == 'r') {
        stats->reads++;
    } else {
        stats->writes++;
    }
    int i;
    tlbe_t *cur;
    for (i = 0; i < tlb_size; i++) {
        cur = tlb + i;
        if (cur->valid == 1 && cur->vpn == vpn) {
            cur->used = 1;
            (current_pagetable + vpn)->frequency++;
            if (rw == 'w') {
                cur->dirty = 1;
            }
            // printf("pfn %"PRIx64"\n", cur->pfn);
            // printf("off %"PRIx64"\n", offset);
            // uint64_t temp2 = (cur->pfn) << (page_size);
            // uint64_t temp = (cur->pfn << (page_size)) | offset;
            // printf("temp2 %"PRIx64"\n", temp2);
            // printf("temp %"PRIx64"\n", temp);
            return (cur->pfn << page_size) | offset;
        }
    }


    // The below function is called if it is a TLB miss
	/* DO NOT MODIFY */
    uint64_t pfn = page_lookup(vpn, offset, rw, stats);
	/*****************/

    // (1) Update the relevant stats
    // (2) Update the TLB with this new mapping
    //      (a) Find an invalid block in the TLB - use it
    //      If you cannot find an invalid block
    //      (i) Run the clock sweep algorithm to find a victim
    //      (ii) Update the current_pagetable at that VPN to dirty if
    //           the evicted TLB entry is dirty
    //      (b) Put the new mapping into the TLB - mark it used


    /********* TODO ************/
    stats->translation_faults++;
    for (i = 0; i < tlb_size; i++) {
        cur = tlb + i;
        if (cur->valid == 0) {
            cur->used = 1;
            cur->vpn = vpn;
            cur->pfn = pfn;
            cur->valid = 1;
            if (rw == 'w') {
                cur->dirty = 1;
            }
            (current_pagetable + vpn)->frequency++;
            return (pfn << page_size) | offset;
        }
    }
    //no invalid
    int j;
    for (j = 0; j < 2; j++) {
        for (i = 0; i < tlb_size; i++) {
            cur = tlb + i;
            if (cur->used == 0) {
                cur->used = 1;
                cur->vpn = vpn;
                cur->pfn = pfn;
                cur->valid = 1;
                if (rw == 'w') {
                    cur->dirty = 1;
                }
                (current_pagetable + vpn)->frequency++;
                return (pfn << page_size) | offset;
            } else {
                cur->used = 0;
            }
        }
    }

    /******* TODO *************/
    // Make sure to return the entire address here, this is just a place holder
    return pfn << page_size | offset;
}

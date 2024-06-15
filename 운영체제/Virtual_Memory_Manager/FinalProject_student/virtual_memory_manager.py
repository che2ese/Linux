import mmap

from global_constants import PAGE_SIZE, PAGE_ENTRIES, FRAME_SIZE, MEM_SIZE
from physical_memory import PhysicalMemory
from tlb_fifo import TLBFifo
from tlb_lru import TLBLru


class PageTable:
    def __init__(self):
        self.table = [-1] * PAGE_ENTRIES

    def get_frame(self, page_number):
        # TODO: 주어진 page number의 frame number는?


    def set_frame(self, page_number, frame_number):
        # TODO: 구현



class VirtualMemoryManager:
    def __init__(self, store_file):
        self.store_file = store_file
        self.store_data = self.map_store_file()
        self.physical_memory = PhysicalMemory()
        self.page_table = PageTable()
        self.tlb = TLBFifo()
        # self.tlb = TLBLru()
        self.stats = {'fault_counter': 0, 'tlb_counter': 0, 'address_counter': 0}

    def map_store_file(self):
        with open(self.store_file, "rb") as f:
            return mmap.mmap(f.fileno(), MEM_SIZE, access=mmap.ACCESS_READ)

    def translate_address(self, virtual_address):
        # virtual to physical memory 변환 횟수 저장
        self.stats['address_counter'] += 1

        page_number = self.get_page_number(virtual_address)
        offset = self.get_offset(virtual_address)
        frame_number = self.tlb.get_frame(page_number)

        if frame_number != -1:
            self.stats['tlb_counter'] += 1
        else:
            frame_number = self.page_table.get_frame(page_number)
            if frame_number == -1:
                self.stats['fault_counter'] += 1
                frame_number = self.handle_page_fault(page_number)

            self.tlb.update(page_number, frame_number)

        physical_address = frame_number * FRAME_SIZE + offset
        value = self.physical_memory.read(physical_address)
        return virtual_address, physical_address, value

    def get_page_number(self, virtual_address):
        return virtual_address >> 8

    def get_offset(self, virtual_address):
        return virtual_address & 255

    def handle_page_fault(self, page_number):
        if self.physical_memory.has_free_frame():
            frame_number = self.physical_memory.get_free_frame()
            page_address = page_number * PAGE_SIZE
            data = self.store_data[page_address:page_address + PAGE_SIZE]
            self.physical_memory.write(frame_number, data)
            self.page_table.set_frame(page_number, frame_number)


            return frame_number
        else:
            raise MemoryError("Out of physical memory")

    def print_statistics(self, output_file):
        fault_rate = self.stats['fault_counter'] / self.stats['address_counter']
        tlb_rate = self.stats['tlb_counter'] / self.stats['address_counter']

        with open(output_file, "a") as out_ptr:
            out_ptr.write(f"Number of Translated Addresses = {self.stats['address_counter']}\n")
            out_ptr.write(f"Page Faults = {self.stats['fault_counter']}\n")
            out_ptr.write(f"Page Fault Rate = {fault_rate:.3f}\n")
            out_ptr.write(f"TLB Hits = {self.stats['tlb_counter']}\n")
            out_ptr.write(f"TLB Hit Rate = {tlb_rate:.3f}\n")


if __name__ == "__main__":
    # Use explicit filenames for input, output, and store files
    input_filename = "addresses.txt"
    store_filename = "BACKING_STORE.bin"
    output_filename = "result_fifo.txt"

    vmm = VirtualMemoryManager(store_filename)

    with open(input_filename, "r") as in_ptr, open(output_filename, "w") as out_ptr:
        for line in in_ptr:
            virtual_address = int(line.strip())
            virtual, physical, value = vmm.translate_address(virtual_address)
            out_ptr.write(f"Virtual address: {virtual} Physical address: {physical} Value: {hex(value & 0xff)}\n")

    vmm.print_statistics(output_filename)

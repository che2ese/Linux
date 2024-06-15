class TLB_LRU:
    def __init__(self):
        self.tlb = []
        self.access_order = []

    def search(self, page_number):
        for entry in self.tlb:
            if entry[0] == page_number:
                self.access_order.remove(page_number)
                self.access_order.append(page_number)
                return entry[1]
        return -1  # TLB 미스

    def update(self, page_number, frame_number):
        if len(self.tlb) >= 16:
            oldest_page = self.access_order.pop(0)
            self.tlb = [entry for entry in self.tlb if entry[0] != oldest_page]
        self.tlb.append((page_number, frame_number))
        self.access_order.append(page_number)



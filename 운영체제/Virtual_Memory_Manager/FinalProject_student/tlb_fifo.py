from global_constants import TLB_ENTRIES


class TLBFifo:
    def __init__(self):
        # TLB 엔트리를 -1로 초기화하여 빈 슬롯을 나타냄
        self.entries = [[-1, -1] for _ in range(TLB_ENTRIES)]
        # back은 최근에 값이 쓰여진 index를 상징.
        self.back = -1

    def get_frame(self, page_number):
        for page, frame in self.entries:
            if page == page_number:
                return frame
        return -1

    def update(self, page_number, frame_number):
        if self.back == -1:
            self.back = 0
        else:
            self.back = (self.back + 1) % TLB_ENTRIES
        self.entries[self.back] = [page_number, frame_number]

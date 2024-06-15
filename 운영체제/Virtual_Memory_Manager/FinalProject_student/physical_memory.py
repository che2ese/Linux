from global_constants import FRAME_SIZE, FRAME_ENTRIES, MEM_SIZE


class PhysicalMemory:
    def __init__(self):
        self.memory = bytearray(MEM_SIZE)
        self.free_frames = list(range(FRAME_ENTRIES))  # Initialize list of free frames

    def read(self, physical_address):
        return self.memory[physical_address]

    def write(self, frame_number, data):
        start = frame_number * FRAME_SIZE
        self.memory[start:start + FRAME_SIZE] = data

    def has_free_frame(self):
        return bool(self.free_frames)

    def get_free_frame(self):
        if self.free_frames:
            frame_number = self.free_frames.pop(0)
            self.free_frames.append(frame_number)  # Move used frame to the end of the list
            return frame_number
        return None

    def return_frame(self, frame_number):
        if frame_number in self.free_frames:
            raise ValueError(f"Frame {frame_number} is already in the list of free frames")
        self.free_frames.append(frame_number)  # Append frame to the end of the list

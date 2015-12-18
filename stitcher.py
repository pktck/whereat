#!/bin/python
"""Test program for stitching images together using OpenCV."""
import sys
import cv2

HEIGHT = 200

class Aligner(object):
    """Alignes images."""

    def __init__(self, image):
        """Return a new Aligner object."""
        self.image = image

    def get_aligned_image(self):
        """Does all the processing and returns an aligned image."""
        lines = self.find_lines()
        self.rotate_crop_scale(lines)

        return self.image

    def find_lines(self):
        """Returns the lines found in the image."""
        return []

    def rotate_crop_scale(self, lines):
        """Rotate the image so the lines are horizontal, crop it,
        and scale it to the defined height."""
        return None

class Stitcher(object):
    """Stitches adjoining images."""

    def __init__(self, image1, image2):
        """Returns a new Stitcher object."""
        self.image1 = image1
        self.image2 = image2

    def get_stitched_image(self):
        """Does all the processing and returns a stitched image."""
        alignment_offset = self.get_alignment_offset()
        stitched_image = self.blend(alignment_offset)

        return stitched_image

    def get_alignment_offset(self):
        """Returns the number of pixels that image2 is offset from image1."""

        return 0

    def blend(self, alignment_offset):
        """Returns the blended image."""
        stitched_image = None

        return stitched_image


def main():
    """To be executed in __main__."""
    filename = sys.argv[1]
    aligner = Aligner(cv2.imread(filename))
    aligned_image = aligner.get_aligned_image()

    return aligned_image







if __name__ == '__main__':
    main()

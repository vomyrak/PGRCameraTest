import matplotlib.pyplot as plt
from matplotlib.figure import Figure
locale = "C:\\Users\\VomyraK\\Documents\\Visual Studio 2015\\Projects\\PGRCameraTest\\PNGReader\\intensity_curve_absolute.txt"


def main():
	data = []
	filename = "Channel 123 absolute luminosity"
	control = []
	error = []
	file = open(locale, "r")
	content = file.read().split()
	error = []
	for i in range(0, len(content)):
		if (i % 2) == 0:
			#control.append(content[i])
			control.append((i / 2) * 0.3)
		else:
			data.append(content[i])
	assert len(control) == len(data)
	fig = plt.figure(facecolor = "#f0f0ed")
	ax = fig.add_subplot(111, title=filename, xlabel="Gain", ylabel="Absolute Intensity")
	#for xy in zip(control, data):                                       # <--
		#ax.annotate('(%s, %s)' % xy, xy=xy, textcoords='data') # <--
	x = list(range(0, 256))
	for i in error:
		del x[i]
		del data[i]
	ax.plot(control, data)
	plt.savefig(filename)
	plt.grid()
	plt.show()
if __name__ == "__main__":
	main()
